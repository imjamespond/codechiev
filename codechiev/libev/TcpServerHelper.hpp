#include "Channel.hpp"
#include "TcpServer.hpp"
#include <base/Logger.hpp>
#include <base/Random.hpp>

#include <assert.h>
#include <errno.h>

#include <event2/event_struct.h>

#include <sys/socket.h>

using namespace codechiev::libev;
using namespace codechiev::base; 


void listener_cb(
    struct evconnlistener *listener,
    evutil_socket_t fd,
    struct sockaddr *sa,
    int socklen,
    void *user_data)
{
  TcpServer *server = static_cast<TcpServer *>(user_data);

  struct bufferevent *bev;
  bev = bufferevent_socket_new(
      server->base,
      fd,
      BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS | BEV_OPT_THREADSAFE);

  if (!bev)
  {
    fprintf(stderr, "Error constructing bufferevent!");
    event_base_loopbreak(server->base);
    return;
  }

  bufferevent_setcb(
      bev,
      readcb,
      writecb,
      eventcb, server);

  bufferevent_enable(bev, EV_READ);
  bufferevent_disable(bev, EV_WRITE);

//   server->onConnect && server->onConnect(bev);
}

void accept_cb(int fd, short flags, void *data)
{
  //This structure is filled in with the address of the peer socket
  sockaddr addr;
  socklen_t socklen;
  int connfd = ::accept(fd, &addr, &socklen);

  TcpServer *server = static_cast<TcpServer *>(data);
  int rand = Random::GetInt(0, (int)(server->workers.size()-1));
  TcpServer::worker_ptr worker = server->workers[rand];
  worker->connfd = connfd;
  event_active(&(worker->acceptev), 0, 0);
  sleep(1);
}

void worker_accept_cb(int fd, short flags, void *data)
{
    TcpServer::Worker *worker = static_cast<TcpServer::Worker *>(data);
    LOG_INFO<<"work accept";
    struct bufferevent *bev;
    bev = bufferevent_socket_new(
        worker->base,
        worker->connfd,
        BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS | BEV_OPT_THREADSAFE);

    if (!bev)
    {
        fprintf(stderr, "Error constructing bufferevent!");
        return;
    }

    bufferevent_setwatermark(
        bev,
        EV_READ | EV_WRITE,
        2,
        1 << 12 //4k
    );
    //On input, a bufferevent does not invoke the user read callback unless there is at least low watermark data in the buffer. If the read buffer is beyond the high watermark, the bufferevent stops reading from the network.
    //On output, the user write callback is invoked whenever the buffered data falls below the low watermark. Filters that write to this bufev will try not to write more bytes to this buffer than the high watermark would allow, except when flushing.

    Channel *channel = new Channel(worker->server, bev);

    bufferevent_setcb(
        bev,
        readcb,
        writecb,
        eventcb,
        channel);

    bufferevent_enable(bev, EV_READ);
    bufferevent_disable(bev, EV_WRITE);

    worker->server->onAccept && worker->server->onAccept(channel);
}
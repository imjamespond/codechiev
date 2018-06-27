#include "TcpServer.hpp"
#include <base/Logger.hpp>

#include <assert.h>
#include <errno.h>

#include <event2/event_struct.h>

#include <sys/socket.h>

using namespace codechiev::libev;
using namespace codechiev::base; 

void listener_cb(struct evconnlistener *,
                 evutil_socket_t,
                 struct sockaddr *,
                 int socklen,
                 void *);
void conn_writecb(struct bufferevent *,
                  void *);
void conn_readcb(struct bufferevent *,
                 void *);
void conn_eventcb(struct bufferevent *,
                  short,
                  void *);

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
      conn_readcb,
      conn_writecb,
      conn_eventcb, server);

  bufferevent_enable(bev, EV_READ);
  bufferevent_disable(bev, EV_WRITE);

  server->onConnect && server->onConnect(bev);
}

void accept_cb(int fd, short flags, void *data)
{
  //This structure is filled in with the address of the peer socket
  sockaddr addr;
  socklen_t socklen;
  int connfd = ::accept(fd, &addr, &socklen);

  TcpServer *server = static_cast<TcpServer *>(data);

  struct bufferevent *bev;
  bev = bufferevent_socket_new(
      server->base,
      connfd,
      BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS | BEV_OPT_THREADSAFE);

  if (!bev)
  {
    fprintf(stderr, "Error constructing bufferevent!");
    event_base_loopbreak(server->base);
    return;
  }

  bufferevent_setcb(
      bev,
      conn_readcb,
      conn_writecb,
      conn_eventcb, server);

  bufferevent_enable(bev, EV_READ);
  bufferevent_disable(bev, EV_WRITE);

  server->onConnect && server->onConnect(bev);
}

void conn_writecb(struct bufferevent *bev, void *user_data)
{
  TcpServer *server = static_cast<TcpServer *>(user_data);

  bufferevent_lock(bev);
  struct evbuffer *output = bufferevent_get_output(bev);
  if (evbuffer_get_length(output) == 0)
  {
    LOG_TRACE << "writecb";

    bufferevent_enable(bev, EV_READ);
    bufferevent_disable(bev, EV_WRITE);

    server->onWrite && server->onWrite(bev);
  }
  bufferevent_unlock(bev);
}

void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
  TcpServer *server = static_cast<TcpServer *>(user_data);

  bufferevent_lock(bev);

  if (events & BEV_EVENT_EOF)
  {
    server->onClose && server->onClose(bev);
  }
  else if (events & BEV_EVENT_ERROR)
  {
    printf("Got an error on the connection: %s\n",
           strerror(errno)); /*XXX win32*/
  }
  /* None of the other events can happen here, since we haven't enabled
	 * timeouts */

  bufferevent_free(bev);
  bufferevent_unlock(bev);
}

void conn_readcb(struct bufferevent *bufev, void *ctx)
{
  TcpServer *server = static_cast<TcpServer *>(ctx);

  bufferevent_lock(bufev);
  // struct evbuffer *evbuf = bufferevent_get_input(bufev);
  // int len = evbuffer_get_length(evbuf);
  // struct iovec iovec; //data will be in iovec.iov_base
  // evbuffer_peek(evbuf, len, NULL, &iovec, 1);

  // LOG_TRACE << "read cb:" << len << "," << evbuffer_pullup(evbuf, len);
  //fwrite(evbuffer_pullup(evbuf, len), len, 1, stdout);

  size_t len(0);
  char data[8];
  while(1) {
    len = bufferevent_read	(bufev, data, 8);
    if(0 == len) {
      break;
    }else{
      server->onRead && server->onRead(bufev, data, len);
    }
  }

  // evbuffer_drain(evbuf, len); //Remove a specified number of bytes data from the beginning of an evbuffer

  bufferevent_unlock(bufev);
}

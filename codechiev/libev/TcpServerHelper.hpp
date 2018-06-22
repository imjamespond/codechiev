#include "TcpServer.hpp"
#include <base/Logger.hpp>

#include <assert.h>
#include <errno.h>

using namespace codechiev::libev;
using namespace codechiev::base;

void listener_cb(struct evconnlistener *, evutil_socket_t,
                 struct sockaddr *, int socklen, void *);
void conn_writecb(struct bufferevent *, void *);
void conn_readcb(struct bufferevent *, void *);
void conn_eventcb(struct bufferevent *, short, void *);

void listener_cb(
    struct evconnlistener *listener,
    evutil_socket_t fd,
    struct sockaddr *sa,
    int socklen,
    void *user_data)
{
  TcpServer *serv = static_cast<TcpServer *>(user_data);

  struct bufferevent *bev;
  bev = bufferevent_socket_new(
      serv->base,
      fd,
      BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS | BEV_OPT_THREADSAFE);

  if (!bev)
  {
    fprintf(stderr, "Error constructing bufferevent!");
    event_base_loopbreak(serv->base);
    return;
  }

  bufferevent_setcb(
      bev,
      conn_readcb,
      conn_writecb,
      conn_eventcb, serv);

  bufferevent_enable(bev, EV_READ);
  bufferevent_disable(bev, EV_WRITE);

  serv->bevMap[fd] = bev;
  //serv->broadcast("foobar");
}

void conn_writecb(struct bufferevent *bev, void *user_data)
{
  bufferevent_lock(bev);

  struct evbuffer *output = bufferevent_get_output(bev);
  if (evbuffer_get_length(output) == 0)
  {
    LOG_INFO << "writecb";

    bufferevent_enable(bev, EV_READ);
    bufferevent_disable(bev, EV_WRITE);
  }

  bufferevent_unlock(bev);
}

void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
  TcpServer *serv = static_cast<TcpServer *>(user_data);

  LOG_INFO << "event: " << events;

  if (events & BEV_EVENT_EOF)
  {
    printf("Connection closed.\n");
  }
  else if (events & BEV_EVENT_ERROR)
  {
    printf("Got an error on the connection: %s\n",
           strerror(errno)); /*XXX win32*/
  }
  /* None of the other events can happen here, since we haven't enabled
	 * timeouts */

  bufferevent_lock(bev);

  evutil_socket_t fd = bufferevent_getfd(bev);
  serv->bevMap.erase(fd);
  LOG_DEBUG << "buffer event map: " << (int)serv->bevMap.size();

  bufferevent_free(bev); //close

  bufferevent_unlock(bev);
}

void conn_readcb(struct bufferevent *bev, void *ctx)
{
  bufferevent_lock(bev);

  struct evbuffer *evbuf = bufferevent_get_input(bev);
  int len = evbuffer_get_length(evbuf);
  struct iovec iovec;
  evbuffer_peek(evbuf, len, NULL, &iovec, 1);

  std::string str((char *)iovec.iov_base, len);
  LOG_INFO << "read:" << len << "," << str;
  //LOG_INFO << "read cb:" << len << "," << evbuffer_pullup(evbuf, len);
  //fwrite(evbuffer_pullup(evbuf, len), len, 1, stdout);
  evbuffer_drain(evbuf, len);

  bufferevent_unlock(bev);
}
#include "TcpServer.hpp"
#include <assert.h>
#include <errno.h>

#include <base/Logger.hpp>

using namespace codechiev::libev;
using namespace codechiev::base;

void listener_cb(struct evconnlistener *, evutil_socket_t,
                 struct sockaddr *, int socklen, void *);
void conn_writecb(struct bufferevent *, void *);
void conn_readcb(struct bufferevent *, void *);
void conn_eventcb(struct bufferevent *, short, void *);

TcpServer::TcpServer(int port): addr(port)
{
  base = event_base_new();
  if (!base)
  {
    fprintf(stderr, "Could not initialize libevent!\n");
    throw 1;
  }

  listener = evconnlistener_new_bind(base,
                                     listener_cb,
                                     (void *)base,
                                     LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
                                     -1,
                                     (struct sockaddr *)&(addr.sin),
                                     sizeof(addr.sin));

  if (!listener)
  {
    fprintf(stderr, "Could not create a listener!\n");
    throw 1;
  }
}

TcpServer::~TcpServer()
{ 
}

void
TcpServer::start()
{
  event_base_dispatch(base);
}

void 
listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
                 struct sockaddr *sa, int socklen, void *user_data)
{
  struct event_base *base = static_cast<struct event_base *>(user_data);
  struct bufferevent *bev;

  bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
  if (!bev)
  {
    fprintf(stderr, "Error constructing bufferevent!");
    event_base_loopbreak(base);
    return;
  }
  bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
  bufferevent_enable(bev, EV_READ);
  bufferevent_disable(bev, EV_WRITE);

  // bufferevent_write(bev, "foobar", strlen("foobar"));

}

void
conn_writecb(struct bufferevent *bev, void *user_data)
{
  struct evbuffer *output = bufferevent_get_output(bev);
  if (evbuffer_get_length(output) == 0)
  {
    LOG_INFO << "flushed answer";
    bufferevent_free(bev);
  }
  else
  {
    bufferevent_enable(bev, EV_READ);
    bufferevent_disable(bev, EV_WRITE);
  }
}

void
conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
  LOG_INFO << events;

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
  bufferevent_free(bev);
}

void 
conn_readcb(struct bufferevent *bev, void *ctx){
  struct evbuffer *evbuf = bufferevent_get_input(bev);
  int len = evbuffer_get_length(evbuf);
  struct iovec iovec;
  evbuffer_peek(evbuf, len, NULL, &iovec, 1);

  //LOG_INFO << "read cb:" << len << "," << (char *)iovec.iov_base;
  LOG_INFO << "read cb:" << len << "," << evbuffer_pullup(evbuf, len);
  //fwrite(evbuffer_pullup(evbuf, len), len, 1, stdout);
  evbuffer_drain(evbuf, len);

  if (len == 0)
  { 
    bufferevent_free(bev);
  }

  bufferevent_enable(bev, EV_WRITE);
  bufferevent_disable(bev, EV_READ);

  bufferevent_write(bev, "foobar", strlen("foobar"));
}
#include "TcpClient.hpp"
#include <base/Logger.hpp>

using namespace codechiev::libev;

void readcb(struct bufferevent *, void *);
void writecb(struct bufferevent *, void *);
void eventcb(struct bufferevent *, short, void *);

TcpClient::TcpClient(const char *addr)
{
  memset(&connect_to_addr, 0, sizeof(connect_to_addr));
  connect_to_addrlen = sizeof(connect_to_addr);
  if (evutil_parse_sockaddr_port(addr,
                                 (struct sockaddr *)&connect_to_addr, &connect_to_addrlen) < 0)
  {
    throw 1;
  }

  base = event_base_new();
  if (!base)
  {
    perror("event_base_new()");
    throw 1;
  }

}

TcpClient::~TcpClient()
{
  event_base_free(base);
}

void
TcpClient::connect()
{
  struct bufferevent *buffev;
  buffev = bufferevent_socket_new(base, -1,
                                  BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);

  if (bufferevent_socket_connect(buffev,
                                 (struct sockaddr *)&connect_to_addr, connect_to_addrlen) < 0)
  {
    perror("bufferevent_socket_connect");
    bufferevent_free(buffev);
    throw 1;
  }

  bufferevent_setcb(buffev, readcb, writecb, eventcb, this);

  event_base_dispatch(base);
}

void TcpClient::write(const char *msg)
{
  //TODO must lock buffer, lock bevMap 

  bufferevent_enable(bev, EV_WRITE);
  bufferevent_disable(bev, EV_READ);

  msg && bufferevent_write(bev, msg, strlen(msg));
}

void eventcb(struct bufferevent *bev, short events, void *user_data)
{
  TcpClient *client = (TcpClient *)user_data;

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
  else if (events & BEV_EVENT_CONNECTED){
    LOG_INFO << "BEV_EVENT_CONNECTED";

    bufferevent_enable(bev, EV_READ);
    bufferevent_disable(bev, EV_WRITE);
    
    client->bev = bev;
    client->write("hello");
  }
  else
  {
    /* None of the other events can happen here, since we haven't enabled
	 * timeouts */
    bufferevent_free(bev);
  }
}

void writecb(struct bufferevent *bev, void *user_data)
{
  struct evbuffer *output = bufferevent_get_output(bev);
  if (evbuffer_get_length(output) == 0)
  {
    LOG_INFO << "writecb"; 
    bufferevent_enable(bev, EV_READ);
    bufferevent_disable(bev, EV_WRITE);
  }
}

void readcb(struct bufferevent *bev, void *ctx)
{
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
    bufferevent_enable(bev, EV_READ);
    bufferevent_disable(bev, EV_WRITE);
  }

  // bufferevent_write(bev, "foobar", strlen("foobar"));
}
//tcp socket

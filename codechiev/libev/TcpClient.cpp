#include "TcpClient.hpp"
#include "TcpClientHelper.hpp"
#include <base/Logger.hpp>

using namespace codechiev::libev;

TcpClient::TcpClient(const char *addr) : TcpEndpoint()
{
  memset(&connect_to_addr, 0, sizeof(connect_to_addr));
  connect_to_addrlen = sizeof(connect_to_addr);
  if (evutil_parse_sockaddr_port(addr,
                                 (struct sockaddr *)&connect_to_addr,
                                 &connect_to_addrlen) < 0)
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
}

void TcpClient::start(int flags)
{
  if (flags)
  {
    event_base_loop(base, flags);
  }
  else
  {
    event_base_dispatch(base);
  }
}

int TcpClient::stop()
{
  return event_base_loopexit(base, NULL);
}

void TcpClient::connect()
{
  struct bufferevent *buffev;
  buffev = bufferevent_socket_new(base, -1,
                                  BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS | BEV_OPT_THREADSAFE); //BEV_OPT_THREADSAFE must be use in multi-thread write

  if (bufferevent_socket_connect(buffev,
                                 (struct sockaddr *)&connect_to_addr,
                                 connect_to_addrlen) < 0)
  {
    perror("bufferevent_socket_connect");
    bufferevent_free(buffev);
    throw 1;
  }

  Channel *channel = new Channel(this, buffev);
  bufferevent_setcb(buffev, readcb, writecb, eventcb, channel);
}

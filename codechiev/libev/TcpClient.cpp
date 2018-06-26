#include "TcpClient.hpp"
#include "TcpClientHelper.hpp"
#include <base/Logger.hpp>

using namespace codechiev::libev;

TcpClient::TcpClient(const char *addr)
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
  if (base)
    event_base_free(base);
}

void
TcpClient::start(int flags)
{
  if(flags){
    event_base_loop(base, flags);
  }else{
    event_base_dispatch(base);
  }
}

int 
TcpClient::stop()
{
  event_base_loopexit(base, NULL);
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

}

void TcpClient::write(bufferevent_struct *bev,
                      const char *msg,
                      size_t size)
{
  bufferevent_enable(bev, EV_WRITE);
  bufferevent_disable(bev, EV_READ);
  msg && bufferevent_write(bev, msg, size);
}


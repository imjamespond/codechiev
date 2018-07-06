#include "TcpClient.hpp"
#include <base/Logger.hpp>

using namespace codechiev::libev;

void _readcb(struct bufferevent *bev, void *ctx)
{
  // TcpClient *client = (TcpClient *)ctx;

  // struct evbuffer *evbuf = bufferevent_get_input(bev);
  // int len = evbuffer_get_length(evbuf);
  // struct iovec iovec;
  // evbuffer_peek(evbuf, len, NULL, &iovec, 1);
  // client->onRead && client->onRead(bev, iovec.iov_base, len);
  // evbuffer_drain(evbuf, len); 

}
//tcp socket

#include "TcpClient.hpp"
#include <base/Logger.hpp>

using namespace codechiev::libev;

void eventcb(struct bufferevent *bev, short events, void *user_data)
{
  TcpClient *client = (TcpClient *)user_data;

  if (events & BEV_EVENT_EOF)
  {
    client->onClose && client->onClose(bev);
    bufferevent_free(bev);
  }
  else if (events & BEV_EVENT_ERROR)
  {
    printf("Got an error on the connection: %s\n",
           strerror(errno)); /*XXX win32*/
    bufferevent_free(bev);
  }
  else if (events & BEV_EVENT_CONNECTED){
    client->onConnect && client->onConnect(bev);

    bufferevent_enable(bev, EV_READ);
    bufferevent_disable(bev, EV_WRITE);
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
  TcpClient *client = (TcpClient *)user_data;

  struct evbuffer *output = bufferevent_get_output(bev);
  if (evbuffer_get_length(output) == 0)
  {
    LOG_TRACE << "writecb";

    bufferevent_enable(bev, EV_READ);
    bufferevent_disable(bev, EV_WRITE);

    client->onWrite && client->onWrite(bev);
  }
}

void readcb(struct bufferevent *bev, void *ctx)
{
  TcpClient *client = (TcpClient *)ctx;

  struct evbuffer *evbuf = bufferevent_get_input(bev);
  int len = evbuffer_get_length(evbuf);
  struct iovec iovec;
  evbuffer_peek(evbuf, len, NULL, &iovec, 1);
  client->onRead && client->onRead(bev, iovec.iov_base, len);
  evbuffer_drain(evbuf, len); 

}
//tcp socket

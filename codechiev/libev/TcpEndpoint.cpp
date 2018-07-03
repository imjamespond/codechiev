#include "TcpEndpoint.hpp"
// #include "TcpServerHelper.hpp"

#include <base/Error.hpp>
#include <base/Logger.hpp>

using namespace codechiev::libev;
using namespace codechiev::base;

TcpEndpoint::TcpEndpoint():base(NULL)
{
  evthread_use_pthreads();
}

void TcpEndpoint::Write(bufferevent_struct *bev,
                      const char *msg,
                      size_t size)
{
  bufferevent_lock(bev);
  bufferevent_enable(bev, EV_WRITE);
  bufferevent_disable(bev, EV_READ);
  msg && bufferevent_write(bev, msg, size);
  bufferevent_unlock(bev);
}

TcpEndpoint::~TcpEndpoint()
{ 
  LOG_DEBUG;
  event_base_free(base);
}

void 
codechiev::libev::eventcb(struct bufferevent *bev, short events, void *user_data)
{
  TcpEndpoint *endpoint = static_cast<TcpEndpoint *>(user_data);

  if (events & BEV_EVENT_EOF)
  {
    bufferevent_lock(bev);
    endpoint->onClose && endpoint->onClose(bev);
    bufferevent_free(bev);
    bufferevent_unlock(bev);
  }
  else if (events & BEV_EVENT_ERROR)
  {
    printf("Got an error on the connection: %s\n", strerror(errno)); /*XXX win32*/
    bufferevent_free(bev);
  }  
  else if (events & BEV_EVENT_CONNECTED)
  {
    bufferevent_enable(bev, EV_READ);
    bufferevent_disable(bev, EV_WRITE);

    endpoint->onConnect && endpoint->onConnect(bev);
  }
  else
  {
    printf("None of the other events can happen here, since we haven't enabled "\
	    "timeouts");
    bufferevent_free(bev);
  }
}

void 
codechiev::libev::readcb(struct bufferevent *bufev, void *ctx)
{
  TcpEndpoint *endpoint = static_cast<TcpEndpoint *>(ctx);

  bufferevent_lock(bufev);
  // struct evbuffer *evbuf = bufferevent_get_input(bufev);
  // int len = evbuffer_get_length(evbuf);
  // struct iovec iovec; //data will be in iovec.iov_base
  // evbuffer_peek(evbuf, len, NULL, &iovec, 1);

  // LOG_TRACE << "read cb:" << len << "," << evbuffer_pullup(evbuf, len);
  //fwrite(evbuffer_pullup(evbuf, len), len, 1, stdout);

  // size_t len(0);
  // char data[128];
  // while(1) {
  //   len = bufferevent_read	(bufev, data, 128);
  //   if(0 == len) {
  //     break;
  //   }else{
  //     endpoint->onRead && endpoint->onRead(bufev, data, len);
  //   }
  // }
  endpoint->onRead && endpoint->onRead(bufev, NULL, 0);

  // evbuffer_drain(evbuf, len); //Remove a specified number of bytes data from the beginning of an evbuffer

  bufferevent_unlock(bufev);
}

void 
codechiev::libev::writecb(struct bufferevent *bev, void *user_data)
{
  TcpEndpoint *endpoint = static_cast<TcpEndpoint *>(user_data);

  bufferevent_lock(bev);
  struct evbuffer *output = bufferevent_get_output(bev);
  if (evbuffer_get_length(output) == 0)
  {
    LOG_TRACE << "writecb";

    bufferevent_enable(bev, EV_READ);
    bufferevent_disable(bev, EV_WRITE);

    endpoint->onWrite && endpoint->onWrite(bev);
  }
  bufferevent_unlock(bev);
}

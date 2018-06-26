#include "TcpServer.hpp"
#include "TcpServerHelper.hpp"

#include <base/Error.hpp>
#include <base/Logger.hpp>

using namespace codechiev::libev;
using namespace codechiev::base;

TcpServer::TcpServer(int port) : addr(port), base(NULL), listener(NULL)
{

  evthread_use_pthreads();

  base = event_base_new();
  if (!base)
  {
    fprintf(stderr, "Could not initialize libevent!\n");
    throw 1;
  }

  listener = evconnlistener_new_bind(base,
                                     listener_cb,
                                     (void *)this,
                                     LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE | LEV_OPT_THREADSAFE,
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
  evconnlistener_free(listener);
  event_base_free(base);
}

void
TcpServer::start()
{
  event_base_dispatch(base);
}

int TcpServer::stop()
{
  event_base_loopexit(base, NULL);
}

void TcpServer::write(bufferevent_struct *bev,
                      const char *msg,
                      size_t size)
{
  bufferevent_lock(bev);
  bufferevent_enable(bev, EV_WRITE);
  bufferevent_disable(bev, EV_READ);
  msg && bufferevent_write(bev, msg, size);
  bufferevent_unlock(bev);
}

TcpServer::TcpServer(): addr(0)
{
  throw Error("default constructor is unavailible.");
}
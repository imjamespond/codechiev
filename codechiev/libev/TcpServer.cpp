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

void TcpServer::stop()
{
  event_base_loopexit(base, NULL);
}

void 
TcpServer::write(int fd, const char * msg)
{
  //TODO must lock buffer, lock bevMap
  struct bufferevent *bev = bevMap[fd];
  if(!bev)
  {
    LOG_DEBUG << "bufferevent is null";
    return;
  }
  bufferevent_lock(bev);
  bufferevent_enable(bev, EV_WRITE);
  bufferevent_disable(bev, EV_READ);
  msg && bufferevent_write(bev, msg, strlen(msg));
  bufferevent_unlock(bev);
}

void 
TcpServer::broadcast(const char * msg)
{
  //TODO must lock buffer, lock bevMap
  BuffereventMap::iterator it;
  for (it = bevMap.begin(); it != bevMap.end(); ++it)
  {
    struct bufferevent *bev = it->second;

    if (!bev)
    {
      LOG_DEBUG << "bufferevent is null";
      return;
    }
    bufferevent_lock(bev);
    bufferevent_enable(bev, EV_WRITE);
    bufferevent_disable(bev, EV_READ);
    msg && bufferevent_write(bev, msg, strlen(msg));
    bufferevent_unlock(bev);
  }
}

TcpServer::TcpServer(): addr(12345)
{
  throw Error("default constructor is unavailible.");
}
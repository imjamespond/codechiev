#include "TcpServer.hpp"
#include "TcpServerHelper.hpp"

#include <base/Error.hpp>
#include <base/Logger.hpp>

using namespace codechiev::libev;
using namespace codechiev::base;

TcpServer::TcpServer(int port) : 
TcpEndpoint(), listenev(NULL), listener(NULL)
{
  set_sock_address(port, addr);

  base = event_base_new();
  if (!base)
  {
    fprintf(stderr, "Could not initialize libevent!\n");
    throw 1;
  }
}

TcpServer::~TcpServer()
{
  LOG_DEBUG;
  if(listener)
  {
    evconnlistener_free(listener);
  }
  if(listenev)
  {
    event_free(listenev);
  } 
}

void
TcpServer::start()
{
  listener = evconnlistener_new_bind(base,
                                     listener_cb,
                                     (void *)this,
                                     LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE | LEV_OPT_THREADSAFE,
                                     -1,
                                     (struct sockaddr *)&(addr),
                                     sizeof(addr)
  );

  if (!listener)
  {
    fprintf(stderr, "Could not create a listener!\n");
    throw 1;
  }

  event_base_dispatch(base);
}

void 
TcpServer::bind()
{
  evutil_socket_t sockfd = ::socket(AF_INET, //SOCK_NONBLOCK
                                  SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, //bitwise value
                                  0);//protocol can be specified as 0
  if (sockfd == -1) {
    perror("socket");
    exit(1);
  }
  evutil_make_socket_nonblocking(sockfd);
  if (::bind(sockfd, (struct sockaddr*)&(addr), sizeof(addr))<0) {
    perror("bind");
    exit(1);
  }
  if (-1 == ::listen(sockfd, 1024))
  {
    perror("listen error"); 
    exit(EXIT_FAILURE);
  } 
  listenev = event_new(base, 
                       sockfd, 
                       EV_READ | EV_PERSIST, 
                       accept_cb, 
                       this
  );
  event_add(listenev, NULL);
  event_base_dispatch(base);
}

int 
TcpServer::stop()
{ 
  return event_base_loopexit(base, NULL);
}

TcpServer::TcpServer()
{
  throw Error("default constructor is unavailible.");
}
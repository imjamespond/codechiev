#include <errno.h>
#include <stdio.h> //perror
#include <sys/socket.h>
/* For sockaddr_in */
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <utils/log.hpp>
#include "server.hpp"

using namespace learn_cpp;

typedef struct sockaddr SockAddressT;
typedef struct sockaddr_in SockAddressInT;
typedef socklen_t SocklenType;

void SetSockAddress(SockAddressInT &sin, int port, const char *host = NULL)
{
  ::memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;   // IPv4 Internet protocols
  sin.sin_port = htons(port); // ::htons(port); not OK with -O2 because it is being replaced with ::(__extension__ ({ register unsigned int __v, ...
  if (host)
    sin.sin_addr.s_addr = ::inet_addr(host);
  else
    sin.sin_addr.s_addr = INADDR_ANY;
}

int GetSockfd()
{
  int sockfd =
      ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
  if (sockfd == -1)
    perror("socket");
  return sockfd;
}

int SetReuseAddr(int sockfd)
{
  int optval = 1;
  if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
  {
    perror("setsockopt");
    return -1;
  }
  return 0;
}

int Listen(int port, const char *host = NULL)
{

  SockAddressInT addr;
  SetSockAddress(addr, port, host);

  int listenfd = GetSockfd();
  if (listenfd == -1)
    return -1;

  if (SetReuseAddr(listenfd) == -1)
    return -1;

  if (::bind(listenfd, (SockAddressT *)&addr, sizeof(SockAddressT)) == -1)
    perror("bind");
  const int LISTEN_BACKLOG = 8192;
  if (::listen(listenfd, LISTEN_BACKLOG) == -1)
    perror("listen");

  return listenfd;
}

int Accept(int fd)
{
  SockAddressInT addr;
  SocklenType addrlen = sizeof(addr);

  int conn = ::accept(fd, (struct sockaddr *)&addr, &addrlen);
  if (conn == -1)
  {
    perror("accept");
  }

  return conn;
}

Server::Server(Loop *loop) : Endpoint(loop), chan(Channel::Create(Listen(12306, "0.0.0.0")))
{
  chan->handle = boost::bind(&Server::handleAccept, this, boost::placeholders::_1, boost::placeholders::_2, loop);
  loop->GetPoll()->CtlAdd(chan, __EVENT_READ__);
}

Server::~Server()
{
  chan->Reset();
}

void Server::handle(const ChannelPtr &chan, int events)
{
  if (events & __EVENT_HUP__)
  { // chan->setClosed();
    Log() << "__EVENT_HUP__" << chan->GetFd();
  }
  else if (events & __EVENT_WRITE__)
  {
    this->handleWrite(chan);
  }
  else if (events & __EVENT_READ__)
  {
    this->handleRead(chan);
  }
}
void Server::handleAccept(const ChannelPtr &chan, int events, Loop *loop)
{

  int fd = Accept(chan->GetFd());
  if (fd < 0)
  {
    Log() << "Accept failed";
  }
  else
  {
    Channel *conn = Channel::Create(fd);
    conn->SetNonblocking();
    conn->handle = boost::bind(&Server::handle, this, boost::placeholders::_1, boost::placeholders::_2);

    loop->GetPoll()->CtlAdd(conn, __EVENT_READ__);

    if (onConnect)
    {
      onConnect(conn->GetPtr());
    }
  }
}
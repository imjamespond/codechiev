#include "Channel.hpp"
#include <stdio.h>
#include <unistd.h>

using namespace codechiev::net;

Channel::Channel(int _sockfd) : sockfd(_sockfd), events(0) {}

/*
  won't trigger or deteted by epoll, not recommended
*/
void Channel::close()
{
  if (-1 == ::close(sockfd))
  {
    perror("close()");
  }
}

void Channel::shutdown()
{
  if (-1 == ::shutdown(sockfd, SHUT_WR))
  {
    perror("shutdown()");
  }
}

void Channel::setNonblocking()
{
  int flags;
  if (-1 == (flags = ::fcntl(sockfd, F_GETFL, 0)))
  {
    flags = 0;
  }
  if (-1 == ::fcntl(sockfd, F_SETFL, flags | O_NONBLOCK))
  {
    perror("fcntl()");
  }
}

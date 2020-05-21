#include "Channel.hpp"
#include "socket.h"
#include <stdio.h>
#include <unistd.h>
#include <base/Logger.hpp>

using namespace codechiev::net;

Channel::Channel(int sockfd) : 
  loop(NULL), 
  connected(false),
  closed(false),
  readDisabled(false),
  sockfd(sockfd), 
  events(0)
{}

Channel::ChannelPtr Channel::Create(int sockfd)
{
  return ChannelPtr(new Channel(sockfd));
}

Channel* Channel::CreateRaw(int sockfd)
{
  return new Channel(sockfd);
}

Channel::~Channel()
{
  close();
}

bool Channel::connectFailed()
{
  int result;
  socklen_t resultLen = sizeof(result);
  if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &result, &resultLen) < 0) {
      return true;
  }

  if (result != 0) {
      return true;
  }

  return false;
}


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

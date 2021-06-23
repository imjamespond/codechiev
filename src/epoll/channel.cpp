#include <stdio.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "channel.hpp"

using namespace learn_cpp;

Channel::Channel(t_fd sockfd) : fd(sockfd), func()
{}

Channel::~Channel()
{}

Channel* Channel::Create(t_fd fd)
{
  return new Channel(fd);
}

void Channel::close()
{
  if (-1 == ::close(fd))
  {
    perror("close()");
  }
}

void Channel::shutdown()
{
  if (-1 == ::shutdown(fd, SHUT_WR))
  {
    perror("shutdown()");
  }
}

void Channel::setNonblocking()
{
  int flags;
  if (-1 == (flags = ::fcntl(fd, F_GETFL, 0)))
  {
    flags = 0;
  }
  if (-1 == ::fcntl(fd, F_SETFL, flags | O_NONBLOCK))
  {
    perror("fcntl()");
  }
}
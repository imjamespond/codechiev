#include <stdio.h>
#include "channel.hpp"

using namespace learn_cpp;

Channel::Channel(t_fd sockfd) : fd(sockfd), func()
{}

Channel::~Channel()
{
  if (-1 == ::close(fd))
  {
    perror("close()");
  }
}

Channel* Channel::Create(t_fd fd)
{
  return new Channel(fd);
}

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <utils/log.hpp>
#include "channel.hpp"

using namespace learn_cpp;

Channel::Channel(t_fd sockfd) : fd(sockfd), handle()
{
}

Channel::~Channel()
{
  Log() << "~Channel() " << fd;
}

Channel *Channel::Create(t_fd fd)
{
  Channel *chan = new Channel(fd);
  chan->self.reset(chan);
  return chan;
}

void Channel::Close()
{
  if (-1 == ::close(fd))
  {
    perror("close()");
  }
}

void Channel::Shutdown()
{
  if (-1 == ::shutdown(fd, SHUT_WR))
  {
    perror("shutdown()");
  }
}

void Channel::SetNonblocking()
{
  int flags = ::fcntl(fd, F_GETFL, 0);
  if (-1 == ::fcntl(fd, F_SETFL, flags | O_NONBLOCK))
  {
    perror("fcntl()");
  }
}
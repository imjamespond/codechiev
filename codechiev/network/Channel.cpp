#include "Channel.hpp"

using namespace codechiev::net;

Channel::Channel(int _fd) : fd(_fd), events(0) {}

void Channel::setNonblocking()
{
  int flags;
  if (-1 == (flags = ::fcntl(fd, F_GETFL, 0)))
    flags = 0;
  ::fcntl(fd, F_SETFD, flags | O_NONBLOCK);
}

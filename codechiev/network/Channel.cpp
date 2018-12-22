#include "Channel.hpp"
#include "socket.h"
#include <stdio.h>
#include <unistd.h>
#include <base/Logger.hpp>

using namespace codechiev::net;

Channel::Channel(int _sockfd) : ptr(this), loop(NULL), sockfd(_sockfd), events(0), action(0)
{}

Channel::Channel(Channel *_ptr, int _sockfd) : ptr(_ptr), loop(NULL), sockfd(_sockfd), events(0), action(0)
{}


Channel::~Channel()
{
  close();
  // LOG_DEBUG << "destroy channel: " << sockfd;
}

bool Channel::check()
{
  int result;
  socklen_t result_len = sizeof(result);
  if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &result, &result_len) < 0) {
      // error, fail somehow, close socket
      return false;
  }

  if (result != 0) {
      // connection failed; error code is in 'result'
      return false;
  }

  return true;
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

// void Channel::stopRead(bool val)
// {
//   // events |= 32;
//   // if (!val) 
//   // {
//   //   events = events ^ 32;
//   // }
//   _stopRead = val;
// }
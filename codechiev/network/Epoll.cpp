#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "Epoll.hpp"
#include "socket.h"
#include <base/Logger.hpp>

using namespace codechiev::net;

int _epoll_create()
{
  int epollfd = ::epoll_create1(EPOLL_CLOEXEC);
  if (epollfd == -1)
  {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }
  return epollfd;
}

Epoll::Epoll() : epChannel(_epoll_create())
{
}

// typedef union epoll_data {
//   void *ptr;
//   int fd;
//   uint32_t u32;
//   uint64_t u64;
// } epoll_data_t;

// struct epoll_event
// {
//   uint32_t events;   /* Epoll events */
//   epoll_data_t data; /* User data variable */
// };

void Epoll::listenCtl(Channel *channel)
{
  // LOG_DEBUG << channel->getFd();

  st_epoll_event ev;
  ev.events = EPOLLIN; // level mode, always notify until read or accept
  // ev.data.fd = channel->getFd(); unable to set both fd and ptr?
  ev.data.ptr = (void *)channel;
  if (_ctl(channel->getFd(), EPOLL_CTL_ADD, ev) == -1)
  {
    perror("epoll_ctl: listen_sock");
    exit(EXIT_FAILURE);
  }
}

void Epoll::connectCtl(Channel *channel)
{
  // LOG_DEBUG << channel->getFd() << channel;

  st_epoll_event ev;
  ev.events = EPOLLIN | EPOLLET; // edge mode
  // ev.data.fd = channel->getFd();
  ev.data.ptr = channel;
  if (_ctl(channel->getFd(), EPOLL_CTL_ADD, ev) == -1)
  {
    perror("epoll_ctl: conn_sock");
    exit(EXIT_FAILURE);
  }
}

int Epoll::_ctl(int fd, int op, st_epoll_event &ev)
{
  return ::epoll_ctl(epChannel.getFd(), op, fd, &ev);
}

void Epoll::wait()
{
  int nfds = ::epoll_wait(epChannel.getFd(), events, MAX_EVENTS, -1);
  if (nfds == -1)
  {
    perror("epoll_wait");
    exit(EXIT_FAILURE);
  }

  for (int n = 0; n < nfds; ++n)
  {
    st_epoll_event &event = events[n];
    if (handler)
    {
      Channel *channel = reinterpret_cast<Channel *>(event.data.ptr);
      updateChannel(channel, event.events);
      handler(channel);
    }

    LOG_DEBUG << "handle ptr: " << event.data.ptr
              << ", events: " << event.events;
  }
  
}

void Epoll::updateChannel(Channel *channel, int events)
{
  if (events & EPOLLIN)
  {
    channel->setReadable();
  }
  else if (events & EPOLLOUT)
  {
    channel->setWritable();
  }
  else if (events & (EPOLLHUP | EPOLLRDHUP))
  {
    channel->setClosable();
  }
}
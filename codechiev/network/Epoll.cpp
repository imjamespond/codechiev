#include "Channel.hpp"
#include "Epoll.hpp"
#include "socket.h"
#include <base/Logger.hpp>

using namespace codechiev::net;

int epoll_create__()
{
  int epollfd = ::epoll_create1(EPOLL_CLOEXEC);
  if (epollfd == -1)
  {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }
  return epollfd;
}

Epoll::Epoll() : epollEvents(1<<2), epChannel(Channel::CreateRaw(epoll_create__()))
{
}

Epoll::~Epoll()
{
  delete epChannel;
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

int Epoll::ctlAdd(Channel *channel, int events)
{
  st_epoll_event ev;
  ev.events = events;
  ev.data.ptr = (void *)channel;
  if (ctl_(channel->getFd(), EPOLL_CTL_ADD, &ev) == -1)
  {
    perror("epoll_ctl: add");
    return -1;
  }
  return 0;
}

int Epoll::ctlDel(Channel *channel)
{
  if (ctl_(channel->getFd(), EPOLL_CTL_DEL, NULL) == -1)
  {
    perror("epoll_ctl: del");
    return -1;
  }
  return 0;
}

int Epoll::ctlMod(Channel *channel, int events)
{
  st_epoll_event ev;
  ev.events = events;
  ev.data.ptr = (void *)channel;
  if (ctl_(channel->getFd(), EPOLL_CTL_MOD, &ev) == -1)
  {
    perror("epoll_ctl: mod");
    return -1;
  }
  return 0;
}

int Epoll::ctl_(int fd, int op, st_epoll_event *ev)
{
  return ::epoll_ctl(epChannel->getFd(), op, fd, ev);
}

void Epoll::wait()
{
  int nfds = ::epoll_wait(epChannel->getFd(), epollEvents.data(), static_cast<int>(epollEvents.size()), -1);
  if (nfds == -1)
  {
    perror("epoll_wait");
  }

  for (int n = 0; n < nfds; ++n)
  {
    if (static_cast<int>(epollEvents.size()) == nfds)
    {
      epollEvents.resize(nfds << 1);
    }

    st_epoll_event &event = epollEvents[n];
    Channel *channel = reinterpret_cast<Channel *>(event.data.ptr);
    update_channel_(channel, event.events);
    if (channel->handler)
    {
      channel->handler(channel->ptr);
    }
  }
}

int Epoll::setReadable(Channel *channel, int events)
{
  return ctlMod(channel, events); //| EPOLLET);
}

int Epoll::setWritable(Channel *channel, int events)
{
  return ctlMod(channel, events); //| EPOLLET);
}


void Epoll::update_channel_(Channel *channel, int events)
{
  if (events & EVENT_HUP_)
  {
    channel->setClosed();
  }
  else if (events & EVENT_WRITE_)
  {
    channel->setWritable();
  }
  else if (events & EVENT_READ_)
  {
    channel->setReadable();
  }
}


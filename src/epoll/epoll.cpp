#include <stdio.h>
#include "channel.hpp"
#include "epoll.hpp"

using namespace learn_cpp;

int 
epoll_create__();
int 
epoll_ctl__(int , int , int , struct_epoll_event *);
void 
handle__(Channel *, int);

Epoll::Epoll() : events(1<<2), fd(epoll_create__())
{
}

Epoll::~Epoll()
{ 
}

int Epoll::ctlAdd(Channel* chan, int events)
{
  struct_epoll_event ev;
  ev.events = events;
  ev.data.ptr = (void *)chan; 
  return epoll_ctl__(this->fd, EPOLL_CTL_ADD,  chan->getFd(), &ev);
}

int Epoll::ctlDel(Channel* chan)
{ 
  int rt = epoll_ctl__(this->fd, EPOLL_CTL_DEL,  chan->getFd(), NULL);
  return rt;
}

int Epoll::ctlMod(Channel* chan, int events)
{
  struct_epoll_event ev;
  ev.events = events;
  ev.data.ptr = (void *)chan; //memory leak caution
  return epoll_ctl__(this->fd, EPOLL_CTL_MOD, chan->getFd(), &ev);
}


void Epoll::wait()
{
  int nfds = ::epoll_wait(this->fd, events.data(), static_cast<int>(events.size()), -1);
  if (nfds == -1)
  {
    perror("epoll_wait error");
  }

  for (int n = 0; n < nfds; ++n)
  {
    if (static_cast<int>(events.size()) == nfds)
    {
      events.resize(nfds << 1);
    }

    struct_epoll_event &event = events[n];
    Channel *chan = reinterpret_cast<Channel *>(event.data.ptr);
    handle__(chan, event.events);
  }
}
 
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


int epoll_ctl__(int epfd, int op, int fd, struct_epoll_event *evp)
{
  int rt = ::epoll_ctl(epfd, op, fd, evp);
  if (rt == -1)
  {
    perror("epoll_ctl error"); 
  }
  return rt;
}

void handle__(Channel *chan, int events)
{ 
  // if (events & EVENT_HUP_)
  //   chan->setClosed();
  // else if (events & EVENT_WRITE_)
  //   chan->setWritable();
  // else if (events & EVENT_READ_)
  //   chan->setReadable();

  if (chan->func) 
  {
    chan->func(events);
  }
}
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

int Epoll::CtlAdd(Channel* chan, int events)
{
  struct_epoll_event ev;
  ev.events = events;
  ev.data.ptr = (void *)chan; 
  return epoll_ctl__(this->fd, EPOLL_CTL_ADD,  chan->GetFd(), &ev);
}

int Epoll::CtlDel(Channel* chan)
{ 
  int rt = epoll_ctl__(this->fd, EPOLL_CTL_DEL,  chan->GetFd(), NULL);
  return rt;
}

int Epoll::CtlMod(Channel* chan, int events)
{
  struct_epoll_event ev;
  ev.events = events;
  ev.data.ptr = (void *)chan; //memory leak caution
  return epoll_ctl__(this->fd, EPOLL_CTL_MOD, chan->GetFd(), &ev);
}


void Epoll::Wait()
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
  // if (events & __EVENT_HUP__)
  //   chan->setClosed();
  // else if (events & __EVENT_WRITE__)
  //   chan->setWritable();
  // else if (events & __EVENT_READ__)
  //   chan->setReadable();

  if (chan->handle) 
  {
    chan->handle(chan->GetPtr(), events);
  }
}
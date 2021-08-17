#include <sys/timerfd.h>
#include <stdio.h>  // perror
#include <errno.h>
#include <boost/bind/bind.hpp>
#include <utils/time.hpp>
#include "epoll.hpp"
#include "timer.hpp"

using namespace learn_cpp;

int 
timerfd_create__();
void 
timerfd_settime__(int , long , long );


Timer::Timer() : chan(Channel::Create(timerfd_create__()))
{
  this->chan->handle = boost::bind(&Timer::handle, this, boost::placeholders::_1 , boost::placeholders::_2); 
}
Timer::~Timer() 
{
  chan->Reset();
}
void Timer::timeout(const Channel::HandleFunc &func, long millis)
{
  this->func = func;

  Time now = Time::NowClock(); 
  timerfd_settime__(this->chan->GetFd(), now.getMillis() + millis, 0l);
} 
void Timer::handle(const ChannelPtr &chan, int events)
{ 
  // if (events & EVENT_HUP_) 
  // { 
  // } 
  // else 
  // if (events & EVENT_WRITE_)
  // { 
  // } 
  // else 
  if (events & __EVENT_READ__)
  {
    for (;;)
    {
      uint64_t exp;
      int len = ::read(chan->GetFd(), &exp, sizeof(uint64_t));
      printf("read len: %d, %lu\n", len, exp);
      if (-1 == len && errno == EAGAIN)
      {
        if (this->func)
        {
          this->func(chan, events);
        }
        break;
      }
    }
  } 
}


int timerfd_create__()
{
  int fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC);
  if (fd == -1)
  {
    ::perror("timerfd_create()");
  }
  return fd;
}

void timerfd_settime__(int timerfd, long timeMillis, long intervalMillis)
{
  struct itimerspec new_value;
  struct itimerspec old_value;

  new_value.it_value.tv_sec = MILLIS_TO_SECS((timeMillis));
  new_value.it_value.tv_nsec = MILLIS_TO_NANOS((timeMillis));

  new_value.it_interval.tv_nsec = MILLIS_TO_NANOS(intervalMillis);
  new_value.it_interval.tv_sec = MILLIS_TO_SECS(intervalMillis);

  if (::timerfd_settime(timerfd, TFD_TIMER_ABSTIME,
                              &new_value /*const struct itimerspec *new_value*/,
                              &old_value /*struct itimerspec * old_value*/) == -1)
  {
    ::perror("timerfd_settime error");
  }
}
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


Timer::Timer() : chan(timerfd_create__()), func()
{
  this->chan.func = boost::bind(&Timer::handle, this, boost::placeholders::_1); 
}
Timer::~Timer() 
{}
void Timer::timeout(const Channel::t_func &func, long millis)
{
  this->func = func;

  Time now = Time::NowClock(); 
  timerfd_settime__(this->chan.getFd(), now.getMillis() + millis, 0l);
} 
void Timer::handle(int events)
{ 
  // if (events & EVENT_HUP_) 
  // { 
  // } 
  // else 
  // if (events & EVENT_WRITE_)
  // { 
  // } 
  // else 
  if (events & EVENT_READ_)
  {
    for (;;)
    {
      uint64_t exp;
      int len = ::read(this->chan.getFd(), &exp, sizeof(uint64_t));
      printf("read len: %d, %lu\n", len, exp);
      if (-1 == len && errno == EAGAIN)
      {
        if (this->func)
        {
          this->func(events);
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
#include "Timer.hpp"
#include "Eventloop.hpp"

#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <errno.h>

#include <boost/bind.hpp>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

int __timerfd_create()
{
  int fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC);
  if (fd == -1)
  {
    perror("timerfd_create()");
  }
  return fd;
}

Timer::Timer() : timerChannel(__timerfd_create())
{
}

void Timer::start(Eventloop<Epoll> &loop)
{
  Epoll::EpollHandler handler = boost::bind(&Timer::_epollHandler, this, _1);
  epoll.setHandler(handler);
  epoll.ctlAdd(&timerChannel, 0);

  loop.loop(&epoll);
}

void Timer::_epollHandler(Channel *channel )
{
  for (;;)
  {
    uint64_t exp;
    int len = ::read(channel->getFd(), &exp, sizeof(uint64_t));

    // LOG_DEBUG << "len: " << len << ", errno: " << errno;
    if (-1 == len)
    {
      if (errno == EAGAIN)
      {
        if (handler)
          handler();
          
        break;
      }
    }
  }

}

// struct timespec
// {
//   time_t tv_sec; /* Seconds */
//   long tv_nsec;  /* Nanoseconds */
// };

// struct itimerspec
// {
//   struct timespec it_interval; /* Interval for periodic timer */
//   struct timespec it_value;    /* Initial expiration */
// };

void Timer::schedule(long afterMillis, long intervalMillis)
{
  struct itimerspec new_value;
  struct itimerspec old_value;
  struct timespec now;
  if (::clock_gettime(CLOCK_REALTIME, &now) == -1)
    perror("clock_gettime");

  new_value.it_value.tv_sec = now.tv_sec + MILLIS_TO_SECS(afterMillis);
  new_value.it_value.tv_nsec = now.tv_nsec + MILLIS_TO_NANOS(afterMillis);

  new_value.it_interval.tv_nsec = MILLIS_TO_NANOS(intervalMillis);
  new_value.it_interval.tv_sec = MILLIS_TO_SECS(intervalMillis);

  if (-1 == ::timerfd_settime(timerChannel.getFd(), TFD_TIMER_ABSTIME,
                              &new_value /*const struct itimerspec *new_value*/,
                              &old_value /*struct itimerspec * old_value*/))
  {
    perror("timerfd_settime()");
  }
}
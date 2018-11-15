#include "Timer.hpp"
#include "Eventloop.hpp"

#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <errno.h>

#include <boost/bind.hpp>
#include <base/Logger.hpp>

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

void Timer::start(Eventloop<Epoll> *loop)
{
  timerChannel.loop = (void *)loop;

  Epoll::EpollHandler handler = boost::bind(&Timer::_epollHandler, this, _1, loop);
  loop->getPoll()->setHandler(handler);
  loop->getPoll()->ctlAdd(&timerChannel, 0);

  loop->loop();
}

void Timer::_epollHandler(Channel *channel, Eventloop<Epoll> *loop)
{
  if (channel->isReadable())
  {
    for (;;)
    {
      uint64_t exp;
      int len = ::read(channel->getFd(), &exp, sizeof(uint64_t));

      // LOG_DEBUG << "len: " << len << ", errno: " << errno;
      if (-1 == len && errno == EAGAIN)
      {
        _execTask();

        break;
      }
    }
  }
}

void Timer::_execTask()
{
  // TODO lock
  Time now = Time::NowClock();
  Tasks::iterator it = tasks.begin();
  Tasks::iterator lastest = tasks.end();
  while (it != tasks.end())
  {
    Task &task = *it;

    // time's up
    if (now > task.when || now == task.when)
    {
      task.func();

      if (task.interval == 0)
      {
        it = tasks.erase(it);
      }
      else if (task.repeat)
      {
        if (--task.repeat == 0)
        {
          it = tasks.erase(it);
        }
        else
        {
          // TODO

          ++it;
        }
      }
      else
      {
        ++it;
      }
    }
    // not yet
    else
    {
      // find out lastest task
      if (lastest == tasks.end())
      { 
        lastest = it; 
      }
      else
      {
        Task &lastestTask = *lastest;
        if (lastestTask.when > task.when)
        { 
          lastest = it;
        }
      }
      ++it;
    }
  }
 
  if (lastest != tasks.end())
  {
    Task &lastestTask = *lastest;
    _schedule(lastestTask.when.getMillis());
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

void Timer::schedule(const TaskFunc &taskFunc, long afterMillis, long intervalMillis, int repeat)
{

  Time now = Time::NowClock();
  Time _time = now.getMillis() + afterMillis;

  Timer::Task task(taskFunc, _time, intervalMillis, repeat);

  if (repeat < 0)
  {
    tasks.clear();
    tasks.push_back(task);
    _schedule(_time.getMillis(), intervalMillis);
  }
  else
  {
    tasks.push_back(task);
    _schedule(now.getMillis()); //active timer instantly
  }
}

void Timer::_schedule(long timeMillis, long intervalMillis)
{
  struct itimerspec new_value;
  struct itimerspec old_value;

  new_value.it_value.tv_sec = MILLIS_TO_SECS((timeMillis));
  new_value.it_value.tv_nsec = MILLIS_TO_NANOS((timeMillis));

  new_value.it_interval.tv_nsec = MILLIS_TO_NANOS(intervalMillis);
  new_value.it_interval.tv_sec = MILLIS_TO_SECS(intervalMillis);

  if (-1 == ::timerfd_settime(timerChannel.getFd(), TFD_TIMER_ABSTIME,
                              &new_value /*const struct itimerspec *new_value*/,
                              &old_value /*struct itimerspec * old_value*/))
  {
    perror("timerfd_settime()");
  }
}
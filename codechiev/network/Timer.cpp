#include "Timer.hpp"
#include "Eventloop.hpp"

#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <errno.h>

#include <boost/bind.hpp>
#include <base/Logger.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

int timerfd_create__()
{
  int fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC);
  if (fd == -1)
  {
    perror("timerfd_create()");
  }
  return fd;
}

Timer::Timer()
{
  timerChannel = Channel::Create(timerfd_create__());
  timerChannel->ptr = timerChannel;
}

Timer::~Timer()
{
  timerChannel->ptr.reset();
}

void Timer::start(Loop *loop)
{
  timerChannel->loop = (void *)loop;

  Handler handler = boost::bind(&Timer::handler_, this, _1, loop);
  loop->getPoll()->setHandler(handler);

  assert(timerChannel->ptr);
  loop->getPoll()->ctlAdd(timerChannel.get(), EPOLLIN);
}

void Timer::stop()
{
  Loop *loop = (Loop *)timerChannel->loop;
  assert(loop);
  loop->getPoll()->ctlDel(timerChannel.get());
}

void Timer::handler_(const Channel::ChannelPtr &channel, Loop *loop)
{
  if (channel->readable())
  {
    for (;;)
    {
      uint64_t exp;
      int len = ::read(channel->getFd(), &exp, sizeof(uint64_t));

      LOG_TRACE << "len: " << len << ", errno: " << errno;
      if (-1 == len && errno == EAGAIN)
      {
        exec_task_();

        break;
      }
    }
  }
}

void Timer::exec_task_()
{
  // TODO using mutex for protecting deque
  
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
      else if (task.repeat > 0)
      {

        if (--task.repeat == 0)
        {
          it = tasks.erase(it);
        }
        else
        {
          task.when = task.when.getMillis() + task.interval;
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
    schedule_(lastestTask.when.getMillis());
  }

  LOG_DEBUG << "task size " << tasks.size();
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
  // TODO using mutex for protecting deque

  Time now = Time::NowClock();
  Time t = now.getMillis() + afterMillis;
  Timer::Task task(taskFunc, t, intervalMillis, repeat);

  if (intervalMillis)
  {
    tasks.clear();
    tasks.push_back(task);
    schedule_(t.getMillis(), intervalMillis);
  }
  else
  {
    tasks.push_back(task);
    schedule_(t.getMillis());
  }
}

void Timer::schedule_(long timeMillis, long intervalMillis)
{
  struct itimerspec new_value;
  struct itimerspec old_value;

  new_value.it_value.tv_sec = MILLIS_TO_SECS((timeMillis));
  new_value.it_value.tv_nsec = MILLIS_TO_NANOS((timeMillis));

  new_value.it_interval.tv_nsec = MILLIS_TO_NANOS(intervalMillis);
  new_value.it_interval.tv_sec = MILLIS_TO_SECS(intervalMillis);

  if (-1 == ::timerfd_settime(timerChannel->getFd(), TFD_TIMER_ABSTIME,
                              &new_value /*const struct itimerspec *new_value*/,
                              &old_value /*struct itimerspec * old_value*/))
  {
    perror("timerfd_settime()");
  }
}
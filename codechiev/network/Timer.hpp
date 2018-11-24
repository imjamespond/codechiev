#ifndef Timer_hpp
#define Timer_hpp

#include "Epoll.hpp"
#include "Eventloop.hpp"

#include <base/Time.hpp>
#include <deque>

namespace codechiev
{
namespace net
{
class Timer
{
public:
  struct Task;
  typedef boost::function<void()> TaskFunc;
  typedef std::deque<Task> Tasks;
  typedef codechiev::base::Time Time;

  struct Task
  {
    TaskFunc func;
    Time when;
    long interval;
    int repeat;

    Task(const TaskFunc &_func, const Time &_when, long _interval, int _repeat)
        : func(_func), when(_when), interval(_interval), repeat(_repeat)
    {
    }
  };

  Timer();

  void start(Eventloop<Epoll> *);
  void schedule(const TaskFunc &, long, long interval = 0, int repeat = 0); // -1 represent repeat infinitely

private:
  Channel *timerChannel;
  Tasks tasks;
  TaskFunc foreverTask;

  void _epoll_handler(const Channel::ChannelPtr &, Eventloop<Epoll> *);
  void _exec_task();
  void _schedule(long, long interval = 0);
};
} // namespace net
} // namespace codechiev

#endif /* Timer_hpp */
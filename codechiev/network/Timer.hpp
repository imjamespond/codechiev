#ifndef Timer_hpp
#define Timer_hpp

#include "Channel.hpp"
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
  typedef Eventloop<Epoll> Loop;
  typedef Epoll::EpollHandler Handler;

  struct Task
  {
    TaskFunc func;
    Time when;
    long interval;
    int repeat;

    Task(const TaskFunc &func, const Time &when, long interval, int repeat)
        : func(func), when(when), interval(interval), repeat(repeat)
    {
    }
  };

  Timer();
  ~Timer();

  void start(Loop *);
  void stop();
  void schedule(const TaskFunc &, long, long intervalMillis = 0, int repeat = 0); //0 represent repeat infinitely

private:
  Channel::ChannelPtr timerChannel;
  Tasks tasks;

  void handler_(const Channel::ChannelPtr &, Loop *);
  void exec_task_();
  void schedule_(long, long intervalMillis = 0);
};
} // namespace net
} // namespace codechiev

#endif /* Timer_hpp */
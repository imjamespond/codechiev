#ifndef Timer_hpp
#define Timer_hpp

#include "Epoll.hpp"
#include "Eventloop.hpp"

namespace codechiev
{
namespace net
{
class Timer
{
public:
  typedef boost::function<void()> HandlerFunc;

  Timer();

  void start(Eventloop<Epoll> &);
  void schedule(long, long);

  inline void setHandler(const HandlerFunc& h) { handler = h;}

private:
  Epoll epoll;
  Channel timerChannel;
  HandlerFunc handler;

  void _epollHandler(Channel *);
};
} // namespace net
} // namespace codechiev

#endif /* Timer_hpp */
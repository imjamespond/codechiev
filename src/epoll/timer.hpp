#ifndef Timer_hpp
#define Timer_hpp

#include "channel.hpp"

namespace learn_cpp
{ 

class Timer
{
public:

  Timer();
  ~Timer();

  void timeout(const Channel::t_func &, long);
  inline Channel *getChan() { return &chan; }
private:
  Channel chan;
  Channel::t_func func;

  void handle(int );
}; 

} // namespace 

#endif /* Timer_hpp */
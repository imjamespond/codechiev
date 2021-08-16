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
  Channel* GetChan(){ return this->_chan; }
private:
  Channel* _chan;
  ChannelPtr chan;
  Channel::t_func func;

  void handle(int );
}; 

} // namespace 

#endif /* Timer_hpp */
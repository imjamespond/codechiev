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

  void timeout(const Channel::HandleFunc &, long);
  Channel* GetChan(){ return this->chan; }
private: 
  Channel* chan;
  Channel::HandleFunc func;

  void handle(const ChannelPtr&, int );
}; 

} // namespace 

#endif /* Timer_hpp */
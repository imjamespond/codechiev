#ifndef Pipe_hpp
#define Pipe_hpp

#include <epoll/channel.hpp>

namespace learn_cpp
{ 

class Pipe
{
public:
  struct PipeFd
  { 
      PipeFd(); 
      int fd[2];
  };

  Pipe();
  ~Pipe();

  // inline Channel *getChan() { return &chan; }
  void readSync();
  void writeSync(const char *, int);
private:
  PipeFd pipe;

  Channel chan0;
  Channel chan1;
  Channel::t_func func;

  void handle(int );
}; 

} // namespace 

#endif /* Pipe_hpp */
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

  inline Channel *getChan0() { return &chan0; }
  inline Channel *getChan1() { return &chan1; }
  int readSync(char *, int);
  void writeSync(const char *, int);
private:
  PipeFd pipe;

  Channel chan0;
  Channel chan1;
  Channel::t_func func;

  void readAsync(int );
  void writeAsync(int );
}; 

} // namespace 

#endif /* Pipe_hpp */
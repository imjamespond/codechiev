#ifndef Pipe_hpp
#define Pipe_hpp
 
#include "channel.hpp"

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

  Pipe(bool aysnc = false);
  ~Pipe();

  inline Channel *GetChan0() { return this->chan0; }
  inline Channel *GetChan1() { return this->chan1; }
  int ReadSync(char *, int);
  void WriteSync(const char *, int);


private:
  PipeFd pipe;
  Channel* chan0;
  Channel* chan1; 
  Channel::HandleFunc func;

}; 

} // namespace 

#endif /* Pipe_hpp */
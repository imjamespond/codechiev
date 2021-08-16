#ifndef Pipe_hpp
#define Pipe_hpp

#include <boost/function.hpp>
#include "./channel.hpp"

namespace learn_cpp
{ 

class Pipe
{
public: 
  typedef boost::function<void(const ChannelPtr &, const char *, int)> OnWriteFunc;
  typedef boost::function<bool(const ChannelPtr &, const char *, int)> OnReadFunc;
  typedef boost::function<void(const ChannelPtr &)> OnReadEndFunc;
  typedef boost::function<void(const ChannelPtr &)> OnWrittenFunc;
  typedef boost::function<void(const ChannelPtr &)> OnCloseFunc;

  struct PipeFd
  { 
      PipeFd(); 
      int fd[2];
  };

  Pipe();
  ~Pipe();

  inline Channel *getChan0() { return this->_chan0; }
  inline Channel *getChan1() { return this->_chan1; }
  int readSync(char *, int);
  void writeSync(const char *, int);

  void ReadAsync(int );
  void WriteAsync(int );

  OnCloseFunc onClose;
  OnReadFunc onRead;
  OnWriteFunc onWrite;
  OnReadEndFunc onReadEnd;
  OnWrittenFunc onWritten; 
private:
  PipeFd pipe;
  Channel* _chan0;
  Channel* _chan1;
  ChannelPtr chan0;
  ChannelPtr chan1;
  Channel::t_func func;
}; 

} // namespace 

#endif /* Pipe_hpp */
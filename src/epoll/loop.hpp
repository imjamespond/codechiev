#ifndef Eventloop_hpp
#define Eventloop_hpp

#include <thread/thread.hpp>
#include <boost/bind/bind.hpp>

namespace learn_cpp
{

template <class T>
class Eventloop
{
public:

  explicit Eventloop() : thread(NULL)
  { 
  }
  ~Eventloop()
  {
    this->join();
  }

  void loop()
  {
    Thread::t_thread_func loopFunc = boost::bind(&Eventloop::runInThread, this);
    thread.setFunc(loopFunc);
    thread.start();
  }

  void stop()
  {
    // thread.cancel();
  }

  void join() 
  {
    thread.join();
  }


  T *getPoll() { return &poll; }

private:
  T poll;
  Thread thread;

  void runInThread()
  {
    // assert(Thread::GetMainId() != Thread::GetCurrentThreadId());
    for (;;)
    {
      try 
      {
        poll.Wait();
      }
      catch (const char *ex)
      {
        printf("loop break: %s\n", ex);
        break;
      }
    }
  }
};

} // namespace learn_cpp

#endif /* Eventloop_hpp */
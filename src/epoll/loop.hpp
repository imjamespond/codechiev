#ifndef Eventloop_hpp
#define Eventloop_hpp

#include <thread/thread.hpp>
#include <boost/bind/bind.hpp>

namespace learn_cpp
{

  template <class T>
  class EventLoop
  {
  public:
    explicit EventLoop() : thread(NULL)
    {
    }
    ~EventLoop()
    {
    }

    void Loop()
    {
      Thread::t_thread_func loopFunc = boost::bind(&EventLoop::runInThread, this);
      thread.setFunc(loopFunc);
      thread.start();
    } 

    void Join()
    {
      thread.join();
    }

    T *GetPoll() { return &poll; }

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
          // printf("loop break: %s\n", ex);
          break;
        }
      }
    }
  };

} // namespace learn_cpp

#endif /* Eventloop_hpp */
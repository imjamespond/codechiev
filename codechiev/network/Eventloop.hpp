#ifndef Eventloop_hpp
#define Eventloop_hpp

#include <base/Thread.hpp>
#include <boost/bind.hpp>

namespace codechiev
{
namespace net
{
class EventloopHelper
{
public:
  static std::string init_thread_name();
};

template <class T>
class Eventloop
{
public:
  typedef codechiev::base::Thread Thread;

  explicit Eventloop() : thread(EventloopHelper::init_thread_name())
  {
  }
  ~Eventloop()
  {
    this->join();
  }

  void loop()
  {
    thread.setFunc(boost::bind(&Eventloop::runInThread, this));
    thread.start();
  }

  void stop()
  {
    if (thread.isStarted())
      thread.cancel();
  }

  void join() 
  {
    if (thread.isStarted())
      thread.join();
  }

  void runInThread()
  {
    assert(Thread::GetMainId() != Thread::GetCurrentThreadId());
    for (;;)
    {
      poll.wait();
    }
  }

  T *getPoll() { return &poll; }

private:
  T poll;

  Thread thread;
};

} // namespace net
} // namespace codechiev

#endif /* Eventloop_hpp */
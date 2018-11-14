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

  explicit Eventloop() : poll(NULL), thread(EventloopHelper::init_thread_name())
  {
  }
  ~Eventloop()
  {
    if (thread.isStarted())
      thread.join();
  }

  void loop(T *t)
  {
    poll = t;
    thread.setFunc(boost::bind(&Eventloop::runInThread, this));
    thread.start();
  }

  void runInThread()
  {
    assert(Thread::GetMainId() != Thread::GetCurrentThreadId());
    for (;;)
    {
      poll->wait();
    }
  }

private:
  T *poll;

  Thread thread;
};
 

} // namespace net
} // namespace codechiev

#endif /* Eventloop_hpp */
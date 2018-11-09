#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <base/Mutex.hpp>
#include "Eventloop.hpp"

using namespace codechiev::base;
using namespace codechiev::net;

static int __count = 0;
static Mutex __count_mutex;

std::string __init_thread_name()
{
  MutexGuard lock(&__count_mutex);
  std::string name = "eventloop-thread-" + boost::lexical_cast<std::string>(__count++);
  return name;
}

Eventloop::Eventloop() : thread(__init_thread_name())
{
}

Eventloop::~Eventloop()
{
  thread.join();
}

void Eventloop::loop(Epoll *epoll)
{
  thread.setFunc(boost::bind(&Eventloop::runInThread, this, epoll));
  thread.start();
}
void Eventloop::runInThread(Epoll *epoll)
{
  assert(Thread::GetMainId() != Thread::GetCurrentThreadId());
  for (;;)
  {
    epoll->wait();
  }
}
#include <boost/lexical_cast.hpp>
#include <base/Mutex.hpp>
#include "Eventloop.hpp"

using namespace codechiev::base;
using namespace codechiev::net;

static int __count = 0;
static Mutex __count_mutex;

std::string EventloopHelper::init_thread_name()
{
  MutexGuard lock(&__count_mutex);
  std::string name = "eventloop-thread-" + boost::lexical_cast<std::string>(__count++);
  return name;
}

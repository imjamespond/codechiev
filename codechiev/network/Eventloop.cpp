#include <boost/lexical_cast.hpp> 
#include <boost/atomic.hpp>
#include "Eventloop.hpp"

using namespace codechiev::base;
using namespace codechiev::net;

boost::atomic<int> count__(0);

std::string EventloopHelper::init_thread_name()
{
  count__.fetch_add(1, boost::memory_order_seq_cst);
  std::string name = "eventloop-thread-" + boost::lexical_cast<std::string>(count__);
  return name;
}

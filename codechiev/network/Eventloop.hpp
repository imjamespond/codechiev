#ifndef Eventloop_hpp
#define Eventloop_hpp

#include <base/Thread.hpp>
#include "Epoll.hpp"

namespace codechiev
{
namespace net
{
class Eventloop
{
public:
  Eventloop();
  ~Eventloop();

  void loop(Epoll *);
  void runInThread(Epoll *);

private:
  codechiev::base::Thread thread;
};
} // namespace net
} // namespace codechiev

#endif /* Eventloop_hpp */
#ifndef Pipe_hpp
#define Pipe_hpp

#include "Epoll.hpp"
#include "Eventloop.hpp"

#include <base/Error.hpp>
#include <base/Logger.hpp>
#include <boost/bind.hpp>

namespace codechiev
{
namespace net
{
  
class Pipe
{
public:
  Pipe();
  // ~Pipe();

  void start(Eventloop<Epoll> *);
  void write(short pipe = 0, void *data = NULL, ssize_t len = 0);

private:

  Channel *p0;
  Channel *p1;
};


} // namespace net
} // namespace codechiev

#endif /* Pipe_hpp */
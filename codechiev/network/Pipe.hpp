#ifndef Pipe_hpp
#define Pipe_hpp

#include "Epoll.hpp"
#include "Eventloop.hpp"

#include <base/Error.hpp>
#include <boost/bind.hpp>

namespace codechiev
{
namespace net
{
template <class T>
class Pipe
{
public:
  Pipe(int *)
  {
    throw codechiev::base::Error("must not call the default constructor");
  }
  ~Pipe()
  {
  }

  void start(Eventloop<T> *);

private:
  void handler(const Channel::ChannelPtr &, Eventloop<T> *);


};

template <>
void Pipe<Epoll>::handler(const Channel::ChannelPtr &, Eventloop<Epoll> *)
{
}

template <>
void Pipe<Epoll>::start(Eventloop<Epoll> *loop)
{
  Epoll::EpollHandler handler = boost::bind(&Pipe<Epoll>::handler, this, _1, loop);
  loop->getPoll()->setHandler(handler);
  loop->loop();
}

// template <>
// Pipe<Epoll>::Pipe(int *pipefd) : p0Channel(pipefd[0]), p1Channel(pipefd[1])
// {
// }

} // namespace net
} // namespace codechiev

#endif /* Pipe_hpp */
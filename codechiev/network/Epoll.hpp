#ifndef Epoll_hpp
#define Epoll_hpp

#include "Channel.hpp"

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <sys/epoll.h>

#define MAX_EVENTS 10

namespace codechiev {
namespace net {
class Epoll {
public:
  Epoll();

  typedef struct epoll_event st_epoll_event;
  typedef boost::shared_ptr<Channel> ChannelPtr;
  typedef boost::function<void(Channel *)> EpollHandler;

  void listenCtl( Channel *);
  void connectCtl( Channel *);
  void wait();
  void handle(st_epoll_event &);

  inline Channel &getChannel() { return epChannel; };
  inline EpollHandler &getHandler() { return handler; };
  inline void setHandler(const EpollHandler& _handler) { handler = _handler; };

private:
  int _ctl(int, int, st_epoll_event &);

  st_epoll_event events[MAX_EVENTS];

  Channel epChannel;
  EpollHandler handler;
};
} // namespace net
} // namespace codechiev

#endif /* Epoll_hpp */
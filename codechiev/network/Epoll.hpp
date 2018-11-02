#ifndef Epoll_hpp
#define Epoll_hpp

#include "Channel.hpp"
#include <boost/shared_ptr.hpp>
#include <sys/epoll.h>

#define MAX_EVENTS 10

namespace codechiev {
namespace net {
class Epoll {
public:
  Epoll();

  typedef boost::shared_ptr<Channel> ChannelPtr;

  void ctl(const ChannelPtr &);
  void wait();

  inline Channel &getChannel() { return epChannel; };

private:
  struct epoll_event ev;
  struct epoll_event events[MAX_EVENTS];
  Channel epChannel;
  ChannelPtr listenChannelPtr;
};
} // namespace net
} // namespace codechiev

#endif /* Epoll_hpp */
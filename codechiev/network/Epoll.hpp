#ifndef Epoll_hpp
#define Epoll_hpp

#include "Channel.hpp"

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <sys/epoll.h>
#include <vector>

// #define MAX_EVENTS 1024*1024

namespace codechiev {
namespace net {
class Epoll {
public:
  Epoll();

  typedef struct epoll_event st_epoll_event;
  typedef std::vector<st_epoll_event> st_vec_epoll_event;
  typedef boost::shared_ptr<Channel> ChannelPtr;
  typedef boost::function<void(Channel *)> EpollHandler;

  int ctlAdd(Channel *, int mode = EPOLLIN | EPOLLET);
  int ctlDel(Channel *);
  int ctlMod(Channel *, int);
  int setReadable(Channel *);
  int setWritable(Channel *);
  void updateChannel(Channel *, int);
  void wait();

  inline Channel &getChannel() { return epChannel; };
  inline EpollHandler &getHandler() { return handler; };

  inline void setHandler(const EpollHandler& _handler) { handler = _handler; };

private:
  int _ctl(int, int, st_epoll_event *);

  st_vec_epoll_event epollEvents;

  Channel epChannel;
  EpollHandler handler;
};
} // namespace net
} // namespace codechiev

#endif /* Epoll_hpp */
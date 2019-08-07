#ifndef Epoll_hpp
#define Epoll_hpp

#include <boost/function.hpp>
#include <sys/epoll.h>
#include <vector>

// #define MAX_EVENTS 1024*1024
#define EVENT_READ_ EPOLLIN
#define EVENT_WRITE_ EPOLLOUT
#define EVENT_HUP_ (EPOLLHUP | EPOLLRDHUP | EPOLLERR)

namespace codechiev {
namespace net {

class Channel;

class Epoll {
public:
  Epoll();
  ~Epoll();

  typedef struct epoll_event st_epoll_event;
  typedef std::vector<st_epoll_event> st_vec_epoll_event;

  int ctlAdd(Channel *, int mode = EPOLLIN | EPOLLET);
  int ctlDel(Channel *);
  int ctlMod(Channel *, int);
  int setReadable(Channel *, int events = EPOLLIN | EPOLLET);
  int setWritable(Channel *, int events = EPOLLOUT | EPOLLET);
  void wait();

  inline Channel* getChannel() { return epChannel; };
  
private:
  int ctl_(int, int, st_epoll_event *);
  void update_channel_(Channel *, int);

  st_vec_epoll_event epollEvents;

  Channel* const epChannel;
  // EpollHandler handler;
};
} // namespace net
} // namespace codechiev

#endif /* Epoll_hpp */
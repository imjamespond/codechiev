#ifndef Epoll_hpp
#define Epoll_hpp

#include <boost/function.hpp>
#include <sys/epoll.h>
#include <vector>

// #define MAX_EVENTS 1024*1024
#define __READABLE__ EPOLLIN
#define __WRITABLE__ EPOLLOUT
//EPOLLHUP no need to add
#define __EVENT_CLOSE__ (EPOLLRDHUP | EPOLLERR)
#define __EVENT_READ__ (EPOLLIN | EPOLLET)
#define __EVENT_WRITE__ (EPOLLOUT | EPOLLET)
#define __EVENT_HUP__ EPOLLHUP

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
  int setReadable(Channel *, int events = __EVENT_READ__);
  int setWritable(Channel *, int events = __EVENT_WRITE__);
  void wait();

  inline Channel* getChannel() { return epChannel; };
  
private:
  int ctl(int, int, st_epoll_event *);
  void updateChannel(Channel *, int);

  st_vec_epoll_event epollEvents;

  Channel* const epChannel;
};
} // namespace net
} // namespace codechiev

#endif /* Epoll_hpp */
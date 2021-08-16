#ifndef Epoll_hpp
#define Epoll_hpp

#include <boost/function.hpp>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <vector>
#include "channel.hpp"

// #define MAX_EVENTS 1024*1024
#define EVENT_READ_ EPOLLIN
#define EVENT_WRITE_ EPOLLOUT
#define EVENT_HUP_ (EPOLLHUP | EPOLLRDHUP | EPOLLERR)

namespace learn_cpp {

// typedef union epoll_data {
//   void *ptr;
//   int fd;
//   uint32_t u32;
//   uint64_t u64;
// } epoll_data_t;

// struct epoll_event
// {
//   uint32_t events;   /* Epoll events */
//   epoll_data_t data; /* User data variable */
// };

typedef struct epoll_event struct_epoll_event;
typedef std::vector<struct_epoll_event> struct_epoll_events;

class Epoll {
public:
  Epoll();
  ~Epoll();

  int CtlAdd(Channel*, int mode = EPOLLIN | EPOLLET); // EPOLLET only notify once
  int CtlDel(Channel*);
  int CtlMod(Channel*, int); 
  void Wait();

  inline Channel::t_fd getEpfd() { return this->fd; };
  
private: 
  struct_epoll_events events;

  const Channel::t_fd fd; 
};

} // namespace

#endif 
#ifndef Channel_hpp
#define Channel_hpp

#include <fcntl.h>
#include <sys/socket.h>
#include <boost/shared_ptr.hpp>

#include <base/Buffer.h>

namespace codechiev {
namespace net {

class Channel {
public:
  typedef boost::shared_ptr<Channel> ChannelPtr; 

  explicit Channel(int);
  explicit Channel(Channel*, int);
  ~Channel();

  // inline void setFd(int sockfd) { this->sockfd = sockfd; }
  inline int getFd() const { return sockfd; } // visit const member

  void setNonblocking();
  bool check();
  void close();
  void shutdown();

  // not thread safe with underscore
  inline void set_event_read() { events = 1 | (events & 24); }
  inline void set_event_write() { events = 2 | (events & 24); }
  inline void set_closed() { events = 4; }
  inline void set_closing() { events |= 8; }
  inline void set_connected() { events |= 16; }

  inline void set_flush_action(bool set = true) { action = (1 | action) ^ (set ? 0 : 1); }
  inline void set_read_action(bool set = true) { action = (2 | action) ^ (set ? 0 : 2); }
  inline void set_write_action(bool set = true) { action = (4 | action) ^ (set ? 0 : 4); }
  inline void enable_read(bool enable = true) { action = (8 | action) ^ (enable ? 8 : 0); }

  inline int event_read() { return (events & 1); }
  inline int event_write() { return (events & 2); }
  inline int is_closed() { return (events & 4); }
  inline int is_closing() { return events & 8; }
  inline int is_connected() { return events & 16; }

  inline int flush_action() { return action & 1; }
  inline int read_action() { return action & 2; }
  inline int write_action() { return action & 4; }
  inline int read_disabled() { return action & 8; } 

  inline int getEvents() { return events;}
  inline int getAction() { return action;}

  const static int BufferSize = 256;

  codechiev::base::Buffer<BufferSize << 1, BufferSize << 4> buffer;
  ChannelPtr ptr;
  void *loop;

private:
  const int sockfd;
  int events;
  int action; //2 for write, 4 for disable read
};
} // namespace net
} // namespace codechiev

#endif /* Channel_hpp */
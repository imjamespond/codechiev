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
  inline void set_readable() { events = 1 | (events & 24); }
  inline void set_writable() { events = 2 | (events & 24); }
  inline void set_closed() { events = 4; }
  inline void set_closing() { events |= 8; }
  inline void set_connected() { events |= 16; }
  void stopRead(bool val=true);
  inline int is_readable() { return (events & 1); }
  inline int is_writable() { return (events & 2); }
  inline int is_closed() { return (events & 4); }
  inline int is_closing() { return events & 8; }
  inline int is_connected() { return events & 16; }
  inline int isStoppedRead() { return _stopRead; }

  inline int getEvents() { return events;}

  codechiev::base::Buffer<8, 1024 * 8> buffer;
  ChannelPtr ptr;
  void *loop;

private:
  const int sockfd;
  int events;
  bool _stopRead;
};
} // namespace net
} // namespace codechiev

#endif /* Channel_hpp */
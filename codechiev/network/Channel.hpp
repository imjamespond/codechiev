#ifndef Channel_hpp
#define Channel_hpp

#include <fcntl.h>
#include <sys/socket.h>

namespace codechiev {
namespace net {
class Channel {
public:
  explicit Channel(int);

  // inline void setFd(int fd) { this->fd = fd; }
  inline int getFd() const { return fd; }

  void setNonblocking();

private:
  const int fd;
  int events;

public:
  inline void setReadable() { events = 1; }
  inline void setWritable() { events = 2; }
  inline void setClosable(){ events = 4; }
  inline int isReadable(){ return (events & 1); }
  inline int isWritable(){ return (events & 2); }
  inline int isClosable(){ return (events & 4); }
};
} // namespace net
} // namespace codechiev

#endif /* Channel_hpp */
#ifndef Channel_hpp
#define Channel_hpp

#include <fcntl.h>
#include <sys/socket.h>

namespace codechiev {
namespace net {
class Channel {
public:
  explicit Channel(int);

  inline void setFd(int fd) { this->fd = fd; }
  inline int getFd() const { return fd; }

private:
  int fd;
};
} // namespace net
} // namespace codechiev

#endif /* Channel_hpp */
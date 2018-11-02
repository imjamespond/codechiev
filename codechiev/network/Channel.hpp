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
  inline void setNonblocking() { ::fcntl(fd, F_SETFD, O_CLOEXEC | O_NONBLOCK); }

private:
  const int fd;
};
} // namespace net
} // namespace codechiev

#endif /* Channel_hpp */
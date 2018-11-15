#ifndef Channel_hpp
#define Channel_hpp

#include <fcntl.h>
#include <sys/socket.h>

#include <base/FixedBuffer.h>

namespace codechiev {
namespace net {
class Channel {
public:
  explicit Channel(int);
  ~Channel();

  // inline void setFd(int sockfd) { this->sockfd = sockfd; }
  inline int getFd() const { return sockfd; }

  void setNonblocking();

private:
  const int sockfd;
  int events;

public:
  void close();
  void shutdown();

  inline void setReadable() { events = 1 | (events & 8); }
  inline void setWritable() { events = 2 | (events & 8); }
  inline void setClosed() { events = 4; }
  inline void setClosable() { events |= 8; }
  inline int isReadable() { return (events & 1); }
  inline int isWritable() { return (events & 2); }
  inline int isClosed() { return (events & 4); }
  inline int isClosable() { return events & 8; }

  codechiev::base::FixedBuffer<4096> buf;
  void *loop;
};
} // namespace net
} // namespace codechiev

#endif /* Channel_hpp */
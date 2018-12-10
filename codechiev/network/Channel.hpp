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

  inline void setReadable() { events = 1 | (events & 24); }
  inline void setWritable() { events = 2 | (events & 24); }
  inline void setClosed() { events = 4; }
  inline void setClosing() { events |= 8; }
  inline void setConnected() { events |= 16; }
  void stopRead(bool val=true);
  inline int isReadable() { return (events & 1); }
  inline int isWritable() { return (events & 2); }
  inline int isClosed() { return (events & 4); }
  inline int isClosing() { return events & 8; }
  inline int isConnected() { return events & 16; }
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
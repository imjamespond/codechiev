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
  const static int BufferSize = 1024;

  typedef codechiev::base::Buffer<BufferSize, BufferSize << 4> Buffer;
  typedef boost::shared_ptr<Channel> ChannelPtr;

  static Channel* CreateRaw(int);
  static ChannelPtr Create(int);
  ~Channel();

  inline int getFd() const { return sockfd; }
  inline int getEvents() { return events;}

  void setNonblocking();
  bool check();
  void close();
  void shutdown();

  // not thread safe with underscore
  inline void setReadable() { events = 1; }
  inline void setWritable() { events = 2; }

  inline void setConnected() { mode = 1; }
  inline void setClosed() { mode = 2; }

  inline int readable() { return (events & 1); }
  inline int writable() { return (events & 2); }
  inline int connected() { return (mode & 1); }
  inline int closed() { return (mode & 2); }


  Buffer buffer;
  ChannelPtr ptr;
  void *loop;

protected:
  explicit Channel(int); 

  const int sockfd;
  int events;
  int mode;
};
} // namespace net
} // namespace codechiev

#endif /* Channel_hpp */
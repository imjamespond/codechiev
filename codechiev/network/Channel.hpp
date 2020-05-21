#ifndef Channel_hpp
#define Channel_hpp

#include <fcntl.h>
#include <sys/socket.h>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <base/Buffer.h>

#include "Eventloop.hpp"
#include "Epoll.hpp"

#define INITIAL_SIZE 128

namespace codechiev {
namespace net {

class Channel {
public:
  const static int BufferSize = INITIAL_SIZE << 10;

  typedef Eventloop<Epoll> Loop;
  typedef codechiev::base::Buffer<INITIAL_SIZE, BufferSize> Buffer;
  typedef boost::shared_ptr<Channel> ChannelPtr;

  typedef boost::function<void(const ChannelPtr &)> Handler;

  static Channel* CreateRaw(int);
  static ChannelPtr Create(int);
  ~Channel();

  inline int getFd() const { return sockfd; }
  inline int getEvents() { return events;}

  void setNonblocking();
  bool connectFailed();
  void shutdown();
 
  inline void setReadable() { events = 1; }
  inline int readable() { return (events & 1); }

  inline void setWritable() { events = 2; }
  inline int writable() { return (events & 2); }

  Handler handler;
  Buffer buffer;
  ChannelPtr ptr;
  Loop *loop;

  bool connected;
  bool closed;
  bool readDisabled;

  inline void disableReading(bool val = true) { readDisabled = val; }
  inline void setClosed() { closed = true; }
  inline void setConnected() { connected = true; }

protected:
  explicit Channel(int); 

  void close();

  const int sockfd;
  int events;
};
} // namespace net
} // namespace codechiev

#endif /* Channel_hpp */
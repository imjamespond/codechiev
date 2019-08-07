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

  typedef boost::function<void(const ChannelPtr &)> OnConnect;
  typedef boost::function<void(const ChannelPtr &, const char *, int)> OnWrite;
  typedef boost::function<bool(const ChannelPtr &, const char *, int)> OnRead;
  typedef boost::function<void(const ChannelPtr &)> OnEndReading;
  typedef boost::function<void(const ChannelPtr &)> OnEndWriting;
  typedef boost::function<void(const ChannelPtr &)> OnClose;

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

  inline void setClosing(bool set = true) { action = (1 | action) ^ (set ? 0 : 1); }
  inline int closing() { return (action & 1); } 
  inline void disableReading(bool set = true) { action = (4|action) ^ (set?0:4); }
  inline int readingDisabled() { return (action & 4); }

  Handler handler;

  Buffer buffer;
  ChannelPtr ptr;
  Loop *loop;

protected:
  explicit Channel(int); 

  const int sockfd;
  int events;
  int mode;
  int action; //4 for read disabled
};
} // namespace net
} // namespace codechiev

#endif /* Channel_hpp */
#ifndef TcpEndpoint_hpp
#define TcpEndpoint_hpp

#include "Channel.hpp"
#include "Epoll.hpp"
#include "Eventloop.hpp"

#include <base/Mutex.hpp>
#include <boost/function.hpp>

namespace codechiev
{
namespace net
{
class TcpEndpoint
{
public:
  typedef Channel::ChannelPtr ChannelPtr;
  typedef boost::function<void(const ChannelPtr &)> OnConnect;
  typedef boost::function<void(const ChannelPtr &, const char *, int)> OnWrite;
  typedef boost::function<bool(const ChannelPtr &, const char *, int)> OnRead;
  typedef boost::function<void(const ChannelPtr &)> OnEndReading;
  typedef boost::function<void(const ChannelPtr &)> OnEndWriting;
  typedef boost::function<void(const ChannelPtr &)> OnClose;

  TcpEndpoint(bool);

  // virtual void start(int) = 0;

  inline void setOnConnect(const OnConnect &func) { onConnect = func; };
  inline void setOnWrite(const OnRead &func) { onRead = func; }; //must not lock
  inline void setOnRead(const OnWrite &func) { onWrite = func; };
  inline void setEndReading(const OnEndReading &func) { onEndReading = func; };
  inline void setEndWriting(const OnEndWriting &func) { onEndWriting = func; }; //must not lock
  inline void setOnCloseFunc(const OnClose &func) { onClose = func; };

  void shutdown(const ChannelPtr &);
  void enableRead(const ChannelPtr &, bool val = true);
  void flush(const ChannelPtr &);
  void flushData(const ChannelPtr &);
  void send(const ChannelPtr &, const char *, int);
  int write(const ChannelPtr &, const char *, int);

protected:
  codechiev::base::Mutex mutex;
  int edge_mode; //false for level triggered, true for edge triggered

  OnConnect onConnect;
  OnClose onClose;
  OnRead onRead;
  OnWrite onWrite;
  OnEndReading onEndReading;
  OnEndWriting onEndWriting;

  void handle_event_(const ChannelPtr &);
  void writing_done_(const ChannelPtr &);
  void close_(Eventloop<Epoll> *, const ChannelPtr &); 
};
} // namespace net
} // namespace codechiev

#endif /* TcpEndpoint_hpp */
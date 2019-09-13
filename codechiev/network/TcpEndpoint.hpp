#ifndef TcpEndpoint_hpp
#define TcpEndpoint_hpp

#include "Channel.hpp"
#include "Epoll.hpp"
#include "Eventloop.hpp"

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

  typedef boost::function<Channel *(int)> CreateChannel;

  TcpEndpoint(bool);

  // virtual void start(int) = 0;

  inline void setOnConnect(const OnConnect &func) { onConnect = func; };
  inline void setOnRead(const OnRead &func) { onRead = func; }; //must not lock
  inline void setOnWrite(const OnWrite &func) { onWrite = func; };
  inline void setEndReading(const OnEndReading &func) { onEndReading = func; };
  inline void setEndWriting(const OnEndWriting &func) { onEndWriting = func; }; //must not lock
  inline void setOnClose(const OnClose &func) { onClose = func; };

  inline void setCreateChannel(const CreateChannel &func) { createChannel = func; }

  void shutdown(const ChannelPtr &);
  void close(const ChannelPtr &);
  void disableReading(const ChannelPtr &, bool disable = true);
  void flush(const ChannelPtr &);
  void send(const ChannelPtr &, const char *, int);
  int write(const ChannelPtr &, const char *, int);

  void handle(const ChannelPtr &);

protected:
  const int edge_mode; //false for level triggered, true for edge triggered
  const int event_read;

  OnConnect onConnect;
  OnClose onClose;
  OnRead onRead;
  OnWrite onWrite;
  OnEndReading onEndReading;
  OnEndWriting onEndWriting;
  
  CreateChannel createChannel;

  void writing_done_(const ChannelPtr &);
};
} // namespace net
} // namespace codechiev

#endif /* TcpEndpoint_hpp */
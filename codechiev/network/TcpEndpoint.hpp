#ifndef TcpEndpoint_hpp
#define TcpEndpoint_hpp

#include "Channel.hpp"
#include "Epoll.hpp"
#include "Eventloop.hpp"

#include <boost/function.hpp>

#define __BUFF_SPAN__ 1024

namespace codechiev
{
namespace net
{
class TcpEndpoint
{
public:
  typedef Channel::ChannelPtr ChannelPtr;
  typedef boost::function<void(const ChannelPtr &)> OnConnectFunc;
  typedef boost::function<void(const ChannelPtr &, const char *, int)> OnWriteFunc;
  typedef boost::function<bool(const ChannelPtr &, const char *, int)> OnReadFunc;
  typedef boost::function<void(const ChannelPtr &)> OnReadEndFunc;
  typedef boost::function<void(const ChannelPtr &)> OnWrittenFunc;
  typedef boost::function<void(const ChannelPtr &)> OnCloseFunc;

  typedef boost::function<Channel *(int)> CreateChannelFunc;

  TcpEndpoint();

  // virtual void start(int) = 0;

  OnConnectFunc onConnect;
  OnCloseFunc onClose;
  OnReadFunc onRead;
  OnWriteFunc onWrite;
  OnReadEndFunc onReadEnd;
  OnWrittenFunc onWritten; 
  
  CreateChannelFunc createChannel; 

  void shutdown(const ChannelPtr &);
  void close(const ChannelPtr &);
  void disableReading(const ChannelPtr &, bool disable = true);
  void flush(const ChannelPtr &);
  void send(const ChannelPtr &, const char *, int);
  int write(const ChannelPtr &, const char *, int);

  void handle(const ChannelPtr &);

protected:
  void handleRead(const ChannelPtr &);
  void handleWrite(const ChannelPtr &);
  void written(const ChannelPtr &);
};
} // namespace net
} // namespace codechiev

#endif /* TcpEndpoint_hpp */
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
  typedef boost::function<void(const ChannelPtr &)> OnConnectFunc;
  typedef boost::function<void(const ChannelPtr &, const char *, int)> OnWriteFunc;
  typedef boost::function<void(const ChannelPtr &, const char *, int)> OnReadFunc;
  typedef boost::function<void(const ChannelPtr &)> OnCompleteWriteFunc;
  typedef boost::function<void(const ChannelPtr &)> OnCloseFunc;
  typedef boost::function<Channel *(int)> CreateChannel;

  TcpEndpoint(bool);

  // virtual void start(int) = 0;

  inline void setOnConnectFunc(const OnConnectFunc &func) { onConnect = func; };
  inline void setOnWriteFunc(const OnWriteFunc &func) { onWrite = func; }; //must not lock
  inline void setOnReadFunc(const OnReadFunc &func) { onRead = func; };
  inline void setOnCompleteWriteFunc(const OnCompleteWriteFunc &func) { onCompleteWrite = func; }; //must not lock
  inline void setOnCloseFunc(const OnCloseFunc &func) { onClose = func; };
  inline void setCreateChannel(const CreateChannel &func) { createChannel = func; }

  void shutdown(const ChannelPtr &);
  void stopRead(const ChannelPtr &, bool val = true);
  int send(const ChannelPtr &, const char *, int, bool flush = true );

protected:
  codechiev::base::Mutex mutex;
  bool edge; //false for level triggered, true for edge triggered

  OnConnectFunc onConnect;
  OnReadFunc onRead;
  OnWriteFunc onWrite;
  OnCompleteWriteFunc onCompleteWrite;
  OnCloseFunc onClose;
  CreateChannel createChannel;

  void _handle_event(const ChannelPtr &);
  void _writing_done(const ChannelPtr &);
  void _close(Eventloop<Epoll> *, const ChannelPtr &);
};
} // namespace net
} // namespace codechiev

#endif /* TcpEndpoint_hpp */
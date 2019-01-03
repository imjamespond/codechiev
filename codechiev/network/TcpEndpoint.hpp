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
  typedef boost::function<void(const ChannelPtr &, const char *, int)> OnPartialWriteFunc;
  typedef boost::function<bool(const ChannelPtr &, const char *, int)> OnPartialReadFunc;
  typedef boost::function<void(const ChannelPtr &)> OnReadFunc;
  typedef boost::function<void(const ChannelPtr &)> OnWriteFunc;
  typedef boost::function<void(const ChannelPtr &)> OnCloseFunc;
  typedef boost::function<Channel *(int)> CreateChannel;

  TcpEndpoint(bool);

  // virtual void start(int) = 0;

  inline void setOnConnectFunc(const OnConnectFunc &func) { onConnect = func; };
  inline void setOnPartialWriteFunc(const OnPartialWriteFunc &func) { onPartialWrite = func; }; //must not lock
  inline void setOnPartialReadFunc(const OnPartialReadFunc &func) { onPartialRead = func; };
  inline void setOnReadFunc(const OnReadFunc &func) { onRead = func; };
  inline void setOnWriteFunc(const OnWriteFunc &func) { onWrite = func; }; //must not lock
  inline void setOnCloseFunc(const OnCloseFunc &func) { onClose = func; };
  inline void setCreateChannel(const CreateChannel &func) { createChannel = func; }

  void shutdown(const ChannelPtr &);
  void enableRead(const ChannelPtr &, bool val = true);
  void flush(const ChannelPtr &);
  void flushData(const ChannelPtr &);
  void send(const ChannelPtr &, const char *, int);
  int write(const ChannelPtr &, const char *, int);

protected:
  codechiev::base::Mutex mutex;
  int edge; //false for level triggered, true for edge triggered

  OnConnectFunc onConnect;
  OnPartialReadFunc onPartialRead;
  OnPartialWriteFunc onPartialWrite;
  OnReadFunc onRead;
  OnWriteFunc onWrite;
  OnCloseFunc onClose;
  CreateChannel createChannel;

  void _handle_event(const ChannelPtr &);
  void _writing_done(const ChannelPtr &);
  void _close(Eventloop<Epoll> *, const ChannelPtr &);
  bool _read_disabled(const ChannelPtr &);
};
} // namespace net
} // namespace codechiev

#endif /* TcpEndpoint_hpp */
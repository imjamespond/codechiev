#ifndef TcpEndpoint_hpp
#define TcpEndpoint_hpp

#include "Channel.hpp"

#include <base/Mutex.hpp>
#include <boost/function.hpp>

namespace codechiev
{
namespace net
{
class TcpEndpoint
{
public:
  typedef boost::function<void(Channel *)> OnConnectFunc;
  typedef boost::function<void(Channel *, const char *, int)> OnWriteFunc;
  typedef boost::function<void(Channel *, const char *, int)> OnReadFunc;
  typedef boost::function<void(Channel *)> OnCloseFunc;
  typedef boost::function<Channel *(int)> CreateChannel;

  TcpEndpoint();

  // virtual void start(int) = 0;

  inline void setOnConnectFunc(const OnConnectFunc &func) { onConnect = func; };
  inline void setOnWriteFunc(const OnWriteFunc &func) { onWrite = func; };
  inline void setOnReadFunc(const OnReadFunc &func) { onRead = func; };
  inline void setOnCloseFunc(const OnCloseFunc &func) { onClose = func; };
  inline void setCreateChannel(const CreateChannel &func) { createChannel = func; }

  void shutdown(Channel *);
  void send(Channel *, const char *, int );

protected:
  codechiev::base::Mutex mutex;

  OnConnectFunc onConnect;
  OnReadFunc onRead;
  OnWriteFunc onWrite;
  OnCloseFunc onClose;
  CreateChannel createChannel;

  void _handleEvent(Channel *); 
  void _writtingDone(Channel *);
};
} // namespace net
} // namespace codechiev

#endif /* TcpEndpoint_hpp */
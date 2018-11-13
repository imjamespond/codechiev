#ifndef TcpEndpoint_hpp
#define TcpEndpoint_hpp

#include "Channel.hpp"

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

  // virtual void start(int) = 0;

  inline void setOnConnectFunc(const OnConnectFunc &func) { onConnect = func; };
  inline void setOnWriteFunc(const OnWriteFunc &func) { onWrite = func; };
  inline void setOnReadFunc(const OnReadFunc &func) { onRead = func; };
  inline void setOnCloseFunc(const OnCloseFunc &func) { onClose = func; };

  virtual void shutdown(Channel *) = 0;

protected:

  OnConnectFunc onConnect;
  OnWriteFunc onWrite;
  OnReadFunc onRead;
  OnCloseFunc onClose;

  void _handler(Channel *);
  
  virtual void _writingDone(Channel *) = 0;
};
} // namespace net
} // namespace codechiev

#endif /* TcpEndpoint_hpp */
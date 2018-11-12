#ifndef TcpEndpoint_hpp
#define TcpEndpoint_hpp

#include <base/Thread.hpp>
#include "Epoll.hpp"

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

  virtual void start(int) = 0;

  inline void setOnConnectFunc(const OnConnectFunc &func) { onConnect = func; };
  inline void setOnWriteFunc(const OnWriteFunc &func) { onWrite = func; };
  inline void setOnReadFunc(const OnReadFunc &func) { onRead = func; };
  inline void setOnCloseFunc(const OnCloseFunc &func) { onClose = func; };

protected:
  Epoll epoll;

  OnConnectFunc onConnect;
  OnWriteFunc onWrite;
  OnReadFunc onRead;
  OnCloseFunc onClose;

  void handler(Channel *);
};
} // namespace net
} // namespace codechiev

#endif /* TcpEndpoint_hpp */
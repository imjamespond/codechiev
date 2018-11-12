#ifndef TcpClient_hpp
#define TcpClient_hpp

#include <base/Thread.hpp> 
#include "Epoll.hpp"

namespace codechiev
{
namespace net
{
class TcpClient : public TcpEndpoint
{
public:
  TcpClient();
  ~TcpClient();

  void connect(const char *, int);
  
};
} // namespace net
} // namespace codechiev

#endif /* TcpClient_hpp */
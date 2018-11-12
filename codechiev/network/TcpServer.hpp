#ifndef TcpServer_hpp
#define TcpServer_hpp

#include "TcpEndpoint.hpp"

namespace codechiev
{
namespace net
{
class TcpServer : public TcpEndpoint
{
public:
  void start(int);
  void epollHandler(Channel *, Channel *);
  void send(Channel *, const char *, int );
};
} // namespace net
} // namespace codechiev

#endif /* TcpServer_hpp */
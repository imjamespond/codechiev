#ifndef TcpServer_hpp
#define TcpServer_hpp

#include "TcpEndpoint.hpp"
#include "Eventloop.hpp"
#include "Epoll.hpp"

namespace codechiev
{
namespace net
{
class TcpServer : public TcpEndpoint
{
public:
  TcpServer();

  void start(int);
  void epollHandler(Channel *, Channel *);
  void send(Channel *, const char *, int );

  void shutdown(Channel *);

protected:
  Epoll epoll;
  Eventloop<Epoll> loop;

  void _writingDone(Channel *);
};
} // namespace net
} // namespace codechiev

#endif /* TcpServer_hpp */
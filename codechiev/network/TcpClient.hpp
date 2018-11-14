#ifndef TcpClient_hpp
#define TcpClient_hpp

#include "TcpEndpoint.hpp"
#include "Eventloop.hpp"
#include "Epoll.hpp"

namespace codechiev
{
namespace net
{
class TcpClient : public TcpEndpoint
{
public:
  TcpClient();

  void epollHandler(Channel *, Channel *);
  void send(Channel *, const char *, int);

  void connect(int, const char *host = "127.0.0.1");
  void start(Eventloop<Epoll> &);

  void shutdown(Channel *);

private:
  Epoll epoll;

  virtual void _writingDone(Channel *);
};
} // namespace net
} // namespace codechiev

#endif /* TcpClient_hpp */
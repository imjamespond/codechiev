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
  explicit TcpClient(Eventloop<Epoll> *);

  void epollHandler(Channel *);
  void send(Channel *, const char *, int);

  void connect(int, const char *host = "127.0.0.1");
  void start();

  void shutdown(Channel *);

private:
  Eventloop<Epoll> *loop;

  virtual void _writtingDone(Channel *);
};
} // namespace net
} // namespace codechiev

#endif /* TcpClient_hpp */
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

  void connect(int, const char *host = "127.0.0.1");
  void connect(Channel *);
  void start();

private:
  Eventloop<Epoll> *loop;
};
} // namespace net
} // namespace codechiev

#endif /* TcpClient_hpp */
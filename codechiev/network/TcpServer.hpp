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
  explicit TcpServer(int, const char *host = NULL);

  void start(Eventloop<Epoll> *, bool isWorker = true);
  void epollHandler(Channel *, Eventloop<Epoll> *);
  void send(Channel *, const char *, int );

  void shutdown(Channel *);

protected:
  Channel listenChannel;

  void _writtingDone(Channel *);
};
} // namespace net
} // namespace codechiev

#endif /* TcpServer_hpp */
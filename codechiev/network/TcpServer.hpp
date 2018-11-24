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

protected:
  void _epoll_handler(const Channel::ChannelPtr &, Eventloop<Epoll> *);

  Channel *listenChannel;
};
} // namespace net
} // namespace codechiev

#endif /* TcpServer_hpp */
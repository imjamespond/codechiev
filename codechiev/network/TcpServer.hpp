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
  typedef Channel::Loop Loop;

  explicit TcpServer(const char *, const char *host = NULL);
  ~TcpServer();

  void init(Loop *);

protected:
  void acceptHandler(const Channel::ChannelPtr &, Loop *);

  Channel* listenChannel;
};
} // namespace net
} // namespace codechiev

#endif /* TcpServer_hpp */
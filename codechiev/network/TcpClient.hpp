#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

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
  typedef Eventloop<Epoll> Loop;
  typedef Epoll::EpollHandler Handler;

  explicit TcpClient(bool edge_mode = false);

  void connect(const char *, const char *host = "127.0.0.1");
  void connect(Channel *);
  void start(Loop *loop);

private:
  void epoll_handler_(const Channel::ChannelPtr &);

  Loop *loop;
};
} // namespace net
} // namespace codechiev

#endif /* TCPCLIENT_HPP */
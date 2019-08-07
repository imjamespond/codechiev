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
  // typedef Epoll::EpollHandler Handler;

  explicit TcpClient(bool edge_mode = false);

  Channel *connect(const char *, const char *host = "127.0.0.1");
  void init(Channel::Loop *);

private:
  void conn_handler_(const Channel::ChannelPtr &);

  Channel::Loop *loop;
};
} // namespace net
} // namespace codechiev

#endif /* TCPCLIENT_HPP */
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
  TcpClient();

  Channel *connect(const char *, const char *host = "127.0.0.1");
  void init(Channel::Loop *);

private:
  void handle(const Channel::ChannelPtr &);

  Channel::Loop *loop;
};
} // namespace net
} // namespace codechiev

#endif /* TCPCLIENT_HPP */
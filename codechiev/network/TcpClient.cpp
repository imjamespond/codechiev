#include "TcpClient.hpp"
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpClient::TcpClient()
{
}

void TcpClient::connect(int port, const char *host)
{
  int conn_sock = get_sockfd();

  /* Code to set up listening socket, 'listen_sock',
    (socket(), bind(), listen()) omitted */

  Channel *connChannel(new Channel(conn_sock));
  Epoll::EpollHandler handler = boost::bind(&TcpClient::epollHandler, this, _1, connChannel);
  epoll.setHandler(handler);

  epoll.ctlAdd(connChannel);
  epoll.setWritable(connChannel);

  Connect(conn_sock, port, host);
}

void TcpClient::start(Eventloop<Epoll> &loop)
{
  loop.loop(&epoll);
}

void TcpClient::epollHandler(Channel *channel, Channel *connChannel)
{
  LOG_DEBUG << "fd " << channel->getFd();
  // if (channel->getFd() == connChannel->getFd())
  // {
  //   LOG_DEBUG << "connect fd " << channel->getFd();
  //   int conn_sock = Accept(channel->getFd());

  //   Channel *conn = new Channel(conn_sock);
  //   conn->setNonblocking();
  //   epoll.ctlAdd(conn);

  //   if (onConnect)
  //     onConnect(conn);
  // }
  // else
  {
    _handler(channel);

    if (channel->isClosed())
    {
      epoll.ctlDel(channel);
      delete channel;
    }
  }
}

void TcpClient::send(Channel *channel, const char *msg, int len)
{
  channel->buf.append(msg, len);

  epoll.setWritable(channel);
}

void TcpClient::shutdown(Channel *channel)
{
  channel->setClosable();
  epoll.setWritable(channel);
}

void TcpClient::_writingDone(Channel *channel)
{
  epoll.setReadable(channel);

  if (channel->isClosable())
  {
    channel->shutdown();
  }
}
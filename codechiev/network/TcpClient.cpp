#include "TcpClient.hpp"
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpClient::TcpClient(Eventloop<Epoll> *_loop) : loop(_loop)
{
  Epoll::EpollHandler handler = boost::bind(&TcpClient::epollHandler, this, _1);
  loop->getPoll()->setHandler(handler);
}

void TcpClient::connect(int port, const char *host)
{
  int conn_sock = get_sockfd();

  Channel *connChannel(new Channel(conn_sock));

  loop->getPoll()->ctlAdd(connChannel);
  loop->getPoll()->setWritable(connChannel);

  Connect(conn_sock, port, host);
}

void TcpClient::start()
{
  loop->loop();
}

void TcpClient::epollHandler(Channel *channel)
{
  // LOG_DEBUG << "fd " << channel->getFd();

  {
    _handler(channel);

    if (channel->isClosed())
    {
      loop->getPoll()->ctlDel(channel);
      delete channel;
    }
  }
}

void TcpClient::send(Channel *channel, const char *msg, int len)
{
  channel->buf.append(msg, len);
  loop->getPoll()
      ->setWritable(channel);
}

void TcpClient::shutdown(Channel *channel)
{
  channel->setClosable();
  loop->getPoll()
      ->setWritable(channel);
}

void TcpClient::_writtingDone(Channel *channel)
{
  loop->getPoll()
      ->setReadable(channel);

  if (channel->isClosable())
  {
    channel->shutdown();
  }
}
#include "TcpClient.hpp"
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpClient::TcpClient(Eventloop<Epoll> *_loop) : TcpEndpoint(), loop(_loop)
{
  Epoll::EpollHandler handler = boost::bind(&TcpClient::epollHandler, this, _1);
  loop->getPoll()->setHandler(handler);
}

void TcpClient::connect(int port, const char *host)
{
  int conn_sock = Connect(port, host);

  Channel *connChannel(new Channel(conn_sock));

  loop->getPoll()->ctlAdd(connChannel);
  loop->getPoll()->setWritable(connChannel);
}

void TcpClient::start()
{
  loop->loop();
}

void TcpClient::epollHandler(Channel *channel)
{
  LOG_DEBUG << "fd: " << channel->getFd() << ", events: " << channel->getEvents();
  
  if (channel->isConnected())
  {
    // _handleEvent(channel);
    LOG_DEBUG << "_handleEvent fd: " << channel->getFd();
  }
  else if (channel->isWritable())
  {
    channel->setConnected();
    channel->setReadable();

    if (onConnect)
      onConnect(channel);
  }

  if (channel->isClosed())
  {
    loop->getPoll()
        ->ctlDel(channel);
    channel->close();
    delete channel;
  }

}
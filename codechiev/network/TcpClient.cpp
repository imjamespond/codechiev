#include "TcpClient.hpp"
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpClient::TcpClient(Eventloop<Epoll> *_loop) : TcpEndpoint(), loop(_loop)
{
  Epoll::EpollHandler handler = boost::bind(&TcpClient::epollHandler, this, _1);
  loop->getPoll()->setHandler(handler);
}

Channel *TcpClient::connect(int port, const char *host)
{
  int conn_sock = Connect(port, host);

  assert(createChannel);
  Channel *conn = createChannel(conn_sock); 

  loop->getPoll()->ctlAdd(conn, EPOLLOUT | EPOLLERR);

  return conn;
}

void TcpClient::start()
{
  loop->loop();
}

void TcpClient::epollHandler(Channel *channel)
{
  // LOG_DEBUG << "fd: " << channel->getFd() << ", events: " << channel->getEvents();
  
  if (channel->isConnected())
  {
    _handleEvent(channel);
    // Time::SleepMillis(5000l);
    // LOG_DEBUG << "_handleEvent fd: " << channel->getFd();
  }
  else if (channel->isWritable())
  {
    if (channel->check())
    {
      channel->setConnected();
      channel->loop = loop;
      
      loop->getPoll()
          ->setReadable(channel);

      if (onConnect)
        onConnect(channel);
    }
    else
    {
      channel->setClosed();
    }
  }

  if (channel->isClosed())
  {
    loop->getPoll()
        ->ctlDel(channel);
    channel->close();
    delete channel;
  }

}
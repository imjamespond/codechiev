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

void TcpClient::connect(int port, const char *host)
{
  int conn_sock = Connect(port, host);

  assert(createChannel);
  Channel *channel = createChannel(conn_sock);

  loop->getPoll()->ctlAdd(channel, EPOLLOUT | EPOLLERR);
}

void TcpClient::connect(Channel * channel)
{
  loop->getPoll()->ctlAdd(channel, EPOLLOUT | EPOLLERR);
}

void TcpClient::start()
{
  loop->loop();
}

void TcpClient::epollHandler(const Channel::ChannelPtr &channel)
{
  // LOG_DEBUG << "fd: " << channel->getFd() << ", events: " << channel->getEvents();
  
  if (channel->isConnected())
  {
    _handle_event(channel);
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
          ->setReadable(channel.get());

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
        ->ctlDel(channel.get());
    channel->close();

    Channel::ChannelPtr _channel;
    channel->ptr.swap(_channel);
    // delete channel;
  }

}
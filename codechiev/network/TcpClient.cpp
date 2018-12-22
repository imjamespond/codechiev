#include "TcpClient.hpp"
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpClient::TcpClient(Eventloop<Epoll> *_loop, bool mode) : TcpEndpoint(mode), loop(_loop)
{
  Epoll::EpollHandler handler = boost::bind(&TcpClient::_epoll_handler, this, _1);
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

void TcpClient::_epoll_handler(const Channel::ChannelPtr &channel)
{
  // LOG_DEBUG << "fd: " << channel->getFd() << ", events: " << channel->getEvents();
  
  if (channel->is_connected())
  {
    _handle_event(channel);
    // Time::SleepMillis(5000l);
    // LOG_DEBUG << "_handleEvent fd: " << channel->getFd();
  }
  else if (channel->is_writable())
  {
    if (channel->check())
    {
      channel->set_connected();
      channel->loop = loop;

      loop->getPoll()
          ->setReadable(channel.get(), EPOLLIN | edge);

      if (onConnect)
        onConnect(channel);
    }
    else
    {
      channel->set_closed();
    }
  }

  if (channel->is_closed())
  {
    _close(loop, channel);
  }

}
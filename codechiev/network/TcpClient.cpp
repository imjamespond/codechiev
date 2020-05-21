#include "TcpClient.hpp"
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpClient::TcpClient() : TcpEndpoint(), loop(NULL)
{
}

void TcpClient::init(Channel::Loop *loop)
{
  this->loop = loop;
}

Channel *TcpClient::connect(const char *port, const char *host)
{
  assert(loop);
  int connFd = Connect(port, host);

  Channel *conn;
  if (createChannel)
  {
    conn = createChannel(connFd);
  }
  else
  {
    conn = Channel::CreateRaw(connFd);
  }

  Channel::Handler handler = boost::bind(&TcpClient::handle, this, _1);
  conn->handler = handler;
  conn->loop = loop;
  conn->ptr = Channel::ChannelPtr(conn);

  loop->getPoll()
      ->ctlAdd(conn, __WRITABLE__ | __EVENT_CLOSE__);

  return conn;
}

void TcpClient::handle(const Channel::ChannelPtr &channel)
{
  // LOG_DEBUG << "fd: " << channel->getFd() << ", events: " << channel->getEvents();

  if (channel->connected)
  {
    TcpEndpoint::handle(channel);
  }
  else if (channel->writable())
  {
    if (!channel->connectFailed())
    {
      channel->setConnected();

      loop->getPoll()
          ->setReadable(channel.get(), __READABLE__);

      if (onConnect)
        onConnect(channel);
    }
    else
    {
      channel->setClosed();

      if (onClose)
      {
        onClose(channel);
      }
      close(channel);
    }
  }
  else if (channel->closed)
  {
    if (onClose)
    {
      onClose(channel);
    }
    close(channel);
  }
}
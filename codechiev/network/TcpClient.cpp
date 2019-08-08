#include "TcpClient.hpp"
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpClient::TcpClient(bool edge_mode) : TcpEndpoint(edge_mode), loop(NULL)
{
}

void TcpClient::init(Channel::Loop * loop)
{
  this->loop = loop;
}

Channel *TcpClient::connect(const char *port, const char *host)
{
  assert(loop);
  int conn_sock = Connect(port, host);

  Channel *conn;
  if (createChannel)
  {
    conn = createChannel(conn_sock);
  }
  else
  {
    conn = Channel::CreateRaw(conn_sock);
  }

  Channel::Handler handler = boost::bind(&TcpClient::conn_handler_, this, _1);
  conn->handler = handler;
  conn->loop = loop;
  conn->ptr = Channel::ChannelPtr(conn);

  loop->getPoll()->ctlAdd(conn, EVENT_WRITE_ | EVENT_HUP_);

  return conn;
}

void TcpClient::conn_handler_(const Channel::ChannelPtr &channel)
{
  // LOG_DEBUG << "fd: " << channel->getFd() << ", events: " << channel->getEvents();

  if (channel->connected())
  {
    handle(channel);
  }
  else if (channel->writable())
  {
    if (channel->check())
    {
      channel->setNonblocking();
      channel->setConnected();

      loop->getPoll()->setReadable(channel.get(), event_read);

      if (onConnect)
        onConnect(channel);
    }
    else
    {
      channel->setClosed();
    }

    if (channel->closed())
    {
      if (onClose)
      {
        onClose(channel);
      }
      close(channel);
    }
  } 
  else if (channel->closed())
  {
    if (onClose)
    {
      onClose(channel);
    }
    close(channel);
  }
}
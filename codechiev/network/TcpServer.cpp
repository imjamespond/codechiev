#include "TcpServer.hpp"
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpServer::TcpServer(const char *port, const char *host, bool edge_mode) : TcpEndpoint(edge_mode),
                                                                           listenChannel(Channel::CreateRaw(Listen(::atoi(port), host)))
{
  listenChannel->ptr = Channel::ChannelPtr(listenChannel);
}

TcpServer::~TcpServer()
{
  LOG_TRACE;
  listenChannel->ptr.reset();
}

void TcpServer::init(Loop *loop, bool main)
{
  Channel::Handler handler = boost::bind(&TcpServer::accept_handler_, this, _1, loop);
  listenChannel->handler = handler;
  listenChannel->loop = loop;
  if (main)
  {
    loop->getPoll()->ctlAdd(listenChannel, EVENT_READ_);
  }
}

void TcpServer::accept_handler_(const Channel::ChannelPtr &channel, Loop *loop)
{

  int conn_sock = Accept(channel->getFd());
  if (conn_sock < 0)
  {
    LOG_ERROR << "Accept failed";
  }
  else
  {
    Channel *conn;
    if (createChannel)
    {
      conn = createChannel(conn_sock);
    }
    else
    {
      conn = Channel::CreateRaw(conn_sock);
    }
    conn->setNonblocking();
    conn->setConnected();
    conn->ptr = Channel::ChannelPtr(conn);
    Channel::Handler handler = boost::bind(&TcpEndpoint::handle, this, _1);
    conn->handler = handler;
    conn->loop = loop;

    loop->getPoll()->ctlAdd(conn, event_read);

    if (onConnect)
    {
      onConnect(conn->ptr);
    }
  }

  if (channel->closed())
  {
    if (onClose)
    {
      onClose(channel);
    }
    close_(loop, channel);
  }
}
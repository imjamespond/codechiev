#include "TcpServer.hpp"
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpServer::TcpServer(const char *port, const char *host) : TcpEndpoint(),
                                                           listenChannel(Channel::CreateRaw(Listen(::atoi(port), host)))
{
  listenChannel->ptr = Channel::ChannelPtr(listenChannel);
}

TcpServer::~TcpServer()
{
  LOG_TRACE;
  listenChannel->ptr.reset();
}

void TcpServer::init(Loop *loop)
{
  Channel::Handler handler = boost::bind(&TcpServer::acceptHandler, this, _1, loop);
  listenChannel->handler = handler;
  listenChannel->loop = loop;
  loop->getPoll()
      ->ctlAdd(listenChannel, __READABLE__);
}

void TcpServer::acceptHandler(const Channel::ChannelPtr &channel, Loop *loop)
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

    loop->getPoll()->ctlAdd(conn, __EVENT_READ__);

    if (onConnect)
    {
      onConnect(conn->ptr);
    }
  }
}
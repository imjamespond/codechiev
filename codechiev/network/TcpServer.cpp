#include "TcpServer.hpp" 
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpServer::TcpServer(const char *port, const char *host, bool edge_mode) : 
  TcpEndpoint(edge_mode), 
  listenChannel(Channel::CreateRaw(Listen(::atoi(port), host)))
{
  listenChannel->ptr = Channel::ChannelPtr(listenChannel);
}

TcpServer::~TcpServer()
{
  LOG_TRACE;
  delete listenChannel;
}

void TcpServer::start(Loop *loop, bool isWorker)
{
  Handler handler = boost::bind(&TcpServer::epoll_handler_, this, _1, loop);
  loop->getPoll()->setHandler(handler);
  if (isWorker)
  {
    loop->getPoll()->ctlAdd(listenChannel, EPOLLIN);
  }
  loop->loop();
}

void TcpServer::epoll_handler_(const Channel::ChannelPtr &channel, Loop *loop)
{
  if (channel->getFd() == listenChannel->getFd())
  {
    int conn_sock = Accept(channel->getFd());
    if (conn_sock < 0)
    {
      LOG_ERROR << "Accept failed";
    }
    else
    {
      Channel *conn = Channel::CreateRaw(conn_sock);
      conn->setNonblocking();
      conn->setConnected();
      conn->ptr = Channel::ChannelPtr(conn);
      conn->loop = loop;
      if (edge_mode)
      {
        loop->getPoll()->ctlAdd(conn); //edge trigger is set as default
      }
      else
      {
        loop->getPoll()->ctlAdd(conn, EPOLLIN); // level trigger EPOLLIN
      }

      if (onConnect)
      {
        onConnect(conn->ptr);
      }
    }
  }
  else
  {
    handle_event_(channel);

    if (channel->closed())
    {
      close_(loop, channel);
    }
  }
}

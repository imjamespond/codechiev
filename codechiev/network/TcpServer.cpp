#include "TcpServer.hpp" 
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpServer::TcpServer(int port, const char *host) : listenChannel(Listen(port, host))
{
}

void TcpServer::start(Eventloop<Epoll> *loop, bool isWorker)
{
  /* Code to set up listening socket, 'listen_sock',
    (socket(), bind(), listen()) omitted */

  Epoll::EpollHandler handler = boost::bind(&TcpServer::_epoll_handler, this, _1, loop);
  loop->getPoll()->setHandler(handler);
  if (isWorker)
  {
    loop->getPoll()->ctlAdd(&listenChannel, EPOLLIN);
  }
  loop->loop();
}

void TcpServer::_epoll_handler(const Channel::ChannelPtr &channel, Eventloop<Epoll> *loop)
{
  if (channel->getFd() == listenChannel.getFd())
  {
    // LOG_DEBUG << "connect fd " << channel->getFd();
    int conn_sock = Accept(channel->getFd());
    if (conn_sock < 0)
    {
      LOG_ERROR << "Accept failed";
    }
    else
    {
      assert(createChannel);
      Channel *conn = createChannel(conn_sock);
      conn->setNonblocking();
      conn->loop = loop;
      conn->setConnected();
      loop->getPoll()
          ->ctlAdd(conn);

      if (onConnect)
        onConnect(conn->ptr);
    }
  }
  else
  {
    _handle_event(channel);

    if (channel->isClosed())
    {
      _close(loop, channel);
    }
  }
}

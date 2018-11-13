#include "TcpServer.hpp" 
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpServer::TcpServer() : loop(&epoll)
{
}

void TcpServer::start(int port = 10000)
{
  int listen_sock = Listen(port);

  /* Code to set up listening socket, 'listen_sock',
    (socket(), bind(), listen()) omitted */

  Channel *listenChannel(new Channel(listen_sock));
  Epoll::EpollHandler handler = boost::bind(&TcpServer::epollHandler, this, _1, listenChannel);
  epoll.setHandler(handler);

  epoll.ctlAdd(listenChannel, 0);

  loop.loop();
}

void TcpServer::epollHandler(Channel *channel, Channel *listenChannel)
{
  if (channel->getFd() == listenChannel->getFd())
  {
    // LOG_DEBUG << "connect fd " << channel->getFd();
    int conn_sock = Accept(channel->getFd());

    Channel *conn = new Channel(conn_sock);
    conn->setNonblocking();
    epoll.ctlAdd(conn);

    if (onConnect)
      onConnect(conn);
  }
  else 
  {
    _handler(channel);

    if(channel->isClosed()){
      epoll.ctlDel(channel);
      delete channel;
    }
  }
}

void TcpServer::send(Channel *channel, const char * msg, int len)
{
    channel->buf.append(msg, len);

    epoll.setWritable(channel);
}

void TcpServer::shutdown(Channel *channel)
{
  channel->setClosable();
  epoll.setWritable(channel);
}

void TcpServer::_writingDone(Channel *channel)
{
  epoll.setReadable(channel);

  if (channel->isClosable())
  {
    channel->shutdown();
  }
}
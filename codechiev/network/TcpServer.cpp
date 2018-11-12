#include "TcpServer.hpp"
#include "Eventloop.hpp"
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

void TcpServer::start(int port = 10000)
{
  int listen_sock = Listen(port);

  /* Code to set up listening socket, 'listen_sock',
    (socket(), bind(), listen()) omitted */

  Channel *listenChannel(new Channel(listen_sock));
  Epoll::EpollHandler handler = boost::bind(&TcpServer::epollHandler, this, _1, listenChannel);
  epoll.setHandler(handler);

  epoll.ctlAdd(listenChannel);

  Eventloop<Epoll> evLoop(&epoll);
  evLoop.loop();
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
    handler(channel);

    if(channel->isClosable()){
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

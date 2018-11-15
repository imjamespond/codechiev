#include "TcpServer.hpp"
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpServer::TcpServer(int port, const char *host) : listenChannel(Listen(port, host))
{
}

void TcpServer::start(Eventloop<Epoll> *loop, bool isWorker)
{
  /* Code to set up listening socket, 'listen_sock',
    (socket(), bind(), listen()) omitted */

  Epoll::EpollHandler handler = boost::bind(&TcpServer::epollHandler, this, _1, loop);
  loop->getPoll()->setHandler(handler);
  if (isWorker)
  {
    loop->getPoll()->ctlAdd(&listenChannel, 0);
  }
  loop->loop();
}

void TcpServer::epollHandler(Channel *channel, Eventloop<Epoll> *loop)
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
      Channel *conn = new Channel(conn_sock);
      conn->setNonblocking();
      conn->loop = loop;
      loop->getPoll()
          ->ctlAdd(conn);

      if (onConnect)
        onConnect(conn);
    }
  }
  else
  {
    _handler(channel);

    if (channel->isClosed())
    {
      loop->getPoll()
          ->ctlDel(channel);
      channel->close();
      delete channel;
    }
  }
}

void TcpServer::send(Channel *channel, const char *msg, int len)
{
  // do not set writable again
  if (!channel->isClosable())
  {
    channel->buf.append(msg, len);
    reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
        ->getPoll()
        ->setWritable(channel);
  }
}

void TcpServer::shutdown(Channel *channel)
{
   // do not set writable again
  if (!channel->isClosable())
  {
    channel->setClosable();
    reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
        ->getPoll()
        ->setWritable(channel);
  } 
}

void TcpServer::_writtingDone(Channel *channel)
{
  reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
      ->getPoll()
      ->setReadable(channel);

  if (channel->isClosable())
  {
    channel->shutdown();
  }
}
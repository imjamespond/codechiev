#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <sys/epoll.h>
#include <boost/bind.hpp>

#include <network/Channel.hpp>
#include <network/Eventloop.hpp>
#include <network/socket.h>

#include <base/Logger.hpp>
#include <base/Keyboard.hpp>

using namespace codechiev::net;
using namespace codechiev::base;

void onConnect(Channel *);
void onRead(Channel *);
void onWrite(Channel *);
void epollHandler(Channel *, Epoll *, Channel *);

void sendMsg(const char *);

int main()
{

  int listen_sock = Listen(12345);

  /* Code to set up listening socket, 'listen_sock',
    (socket(), bind(), listen()) omitted */

  Channel *listenChannel(new Channel(listen_sock));
  Epoll epoll;

  Epoll::EpollHandler handler = boost::bind(&epollHandler, _1, &epoll, listenChannel);
  epoll.setHandler(handler);

  epoll.listenCtl(listenChannel);

  Eventloop evLoop;
  evLoop.loop(&epoll);
}

void onConnect(Channel *conn)
{
  LOG_INFO << "connected channel: " << conn->getFd();
}

void sendMsg(const char * abc)
{
  
}

void epollHandler(Channel *channel, Epoll *epoll, Channel *listenChannel)
{
  if (channel->getFd() == listenChannel->getFd())
  {
    LOG_DEBUG << "connect fd " << channel->getFd();
    int conn_sock = Accept(channel->getFd());

    Channel *conn = new Channel(conn_sock);
    // setnonblocking(conn_sock);
    conn->setNonblocking();
    epoll->connectCtl(conn);
    onConnect(conn);
  }
  else
  {
    if (channel->isReadable())
    {
      char buf[4];
      size_t buf_len = sizeof buf;
      for (;;)
      {
        ::memset(buf, 0, buf_len);
        ssize_t len = ::read(channel->getFd(), buf, buf_len);

        if (len)
        {
          LOG_DEBUG << "active fd: " << channel->getFd() << ", buf: " << buf;
          if ((size_t)len < buf_len)
          {
            break;
          }
        }
        else
        {
          LOG_DEBUG << "close fd: " << channel->getFd();
          break;
        }

        LOG_DEBUG << "len: " << len << ", errno: " << errno;
        if (-1 == len)
        {
          break;
        }
      }
    }
  }
}
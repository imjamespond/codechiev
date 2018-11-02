#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <boost/bind.hpp>

#include <network/Channel.hpp>
#include <network/Eventloop.hpp>
#include <network/socket.h>

#include <base/Logger.hpp>

using namespace codechiev::net;
using namespace codechiev::base;

void epollHandler(Channel *channel, Epoll *epoll, Channel *listenChannel)
{
  if (channel->getFd() == listenChannel->getFd())
  {
    LOG_DEBUG << "connect fd " << channel->getFd();
    int conn_sock = Accept(channel->getFd());

    epoll->connectCtl(new Channel(conn_sock));
  }
  else
  {
    // do_use_fd(events[n].data.fd);
    LOG_DEBUG << "active fd " << channel->getFd();
  }
}

int main() { 


  int listen_sock = Listen(12345);

  /* Code to set up listening socket, 'listen_sock',
    (socket(), bind(), listen()) omitted */

  Channel *listenChannel(new Channel(listen_sock));
  Epoll epoll;

  Epoll::EpollHandler handler = boost::bind(&epollHandler, _1, &epoll, listenChannel);
  epoll.setHandler(handler);

  epoll.listenCtl(listenChannel);

  Eventloop evLoop;
  evLoop.loop(epoll);
}
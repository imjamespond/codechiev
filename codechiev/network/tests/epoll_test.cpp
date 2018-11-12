#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <sys/epoll.h>
#include <boost/bind.hpp>

#include <network/Channel.hpp>
#include <network/Eventloop.hpp>
#include <network/socket.h>

#include <base/Time.hpp>
#include <base/Logger.hpp>
#include <base/Keyboard.hpp>

using namespace codechiev::net;
using namespace codechiev::base;

void onConnect(Channel *);
void onRead(Channel *);
void onWrite(Channel *);
void epollHandler(Channel *, Epoll *, Channel *);

void input();

Channel *Conn = NULL;
Epoll *Ep = NULL;

int main()
{
  //input thread
  Thread inputTread("input", boost::bind(&input));
  inputTread.start();


  int listen_sock = Listen(12345);

  /* Code to set up listening socket, 'listen_sock',
    (socket(), bind(), listen()) omitted */

  Channel *listenChannel(new Channel(listen_sock));
  Epoll epoll;
  Ep = &epoll;

  Epoll::EpollHandler handler = boost::bind(&epollHandler, _1, &epoll, listenChannel);
  epoll.setHandler(handler);

  epoll.ctlAdd(listenChannel);

  Eventloop evLoop;
  evLoop.loop(&epoll);

  inputTread.join();
}

void input()
{
  char string[256];
  for (;;)
  {
    if (keyboard::fgets(string, 100) != NULL)
    {
      printf("fgets: %s\n", string);
      if ( strcmp(string, "close\n") == 0)
      {
        if (Conn)
        {
          Conn->shutdown();
        }
      }
      else if (Conn)
      {
        Ep->setWritable(Conn);
      }
    }
  }
}

void onConnect(Channel *conn)
{
  LOG_INFO << "connected channel: " << conn->getFd();
}

void epollHandler(Channel *channel, Epoll *epoll, Channel *listenChannel)
{
  if (channel->getFd() == listenChannel->getFd())
  {
    LOG_DEBUG << "connect fd " << channel->getFd();
    int conn_sock = Accept(channel->getFd());

    Conn = new Channel(conn_sock);
    // setnonblocking(conn_sock);
    Conn->setNonblocking();
    epoll->ctlAdd(Conn);
    onConnect(Conn);
  }
  else
  {
    if (channel->isReadable())
    {
      char buf[32];
      size_t buf_len = sizeof buf;
      for (;;)
      {
        ::memset(buf, 0, buf_len);
        ssize_t len = ::read(channel->getFd(), buf, buf_len);

        LOG_DEBUG << "fd: " << channel->getFd() << ", len: " << len << ", errno: " << errno;

        if (len > 0)
        {
          LOG_DEBUG << "buf: " << buf;
          Time::SleepMillis(10000l);
        }
        else if(len)
        {
          if (-1 == len && errno == EAGAIN)
          {
            LOG_DEBUG << "EAGAIN";
            break;
          }
        }
        else
        {
          LOG_DEBUG << "close fd: " << channel->getFd();
          break;
        }


      }
    }
    //write
    else if(channel->isWritable())
    {
      LOG_DEBUG << "writable fd: " << channel->getFd();
      Ep->setReadable(channel);
    }
    //close
    else if (channel->isClosable())
    {
      LOG_DEBUG << "shut down writing half of connection fd : " << channel->getFd();

    }
  }
}
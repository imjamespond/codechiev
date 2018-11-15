#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <errno.h>
#include <sys/epoll.h>
// #include <signal.h>
#include <boost/bind.hpp>

#include <network/TcpServer.hpp>
#include <network/TcpClient.hpp>
#include <network/Timer.hpp>

#include <base/Time.hpp>
#include <base/Logger.hpp>
#include <base/Keyboard.hpp>

using namespace codechiev::net;
using namespace codechiev::base;

void onConnect(Channel *);
void onRead(Channel *, const char *, int, TcpServer *);
void onWrite(Channel *, const char *, int, TcpServer *);
void onClose(Channel *);

void onClientConnect(Channel *);

void onStatistic();
 
int port = 12345;
int host = "192.168.0.254";

long cliRecived = 0;
long cliRecived = 0;
long servRecived = 0;
long servSend = 0;

Eventloop<Epoll> loop;
timer.setHandler(boost::bind(&onStatistic));
timer.start(&loop);

void input();

int main(int num, const char **args)
{
  if (num > 1)
  {
    host = args[1];
  }
  if (num > 2)
  {
    port = ::atoi(args[2]); 
  }

  LOG_INFO << "host: " << host << ", port: " << port;

  Eventloop<Epoll> serv1Loop; 
  Eventloop<Epoll> cliLoop;

  TcpServer serv2(port, host);

  serv1.setOnConnectFunc(boost::bind(&onConnect, _1));
  serv1.setOnCloseFunc(boost::bind(&onClose, _1));
  serv1.setOnReadFunc(boost::bind(&onRead, _1, _2, _3, &serv2));
  serv1.setOnWriteFunc(boost::bind(&onWrite, _1, _2, _3, &serv2));
  serv1.start(serv2Loop, port, host);

  client.setOnConnectFunc(boost::bind(&onClientConnect, _1));
  client.start(cliLoop);

  input();

  return 1;
}

void input()
{
  char string[256];
  for (;;)
  {
    if (keyboard::fgets(string, 100) != NULL)
    {
      printf("fgets: %s", string);
      if (::strcmp(string, "count\n") == 0)
      {
        LOG_INFO << client_num;
      }
      else if (::strcmp(string, "connect\n") == 0)
      {
        for (int i = 0; i < client_total; ++i)
        {
          client.connect(12345);
        }
      }
    }
  }

}

void onConnect(Channel *channel)
{
  client_num++;
  // LOG_INFO << "connect fd: " << channel->getFd();
}
void onRead(Channel *channel, const char *buf, int len, TcpServer * serv)
{
}
void onWrite(Channel *channel, const char *msg, int len, TcpServer *serv)
{

}
void onClose(Channel *channel)
{
}

void onClientConnect(Channel *channel)
{
}
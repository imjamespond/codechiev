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

void onConnect(Channel *, TcpServer *);
void onRead(Channel *, const char *, int, TcpServer *);
void onWrite(Channel *, const char *, int, TcpServer *);
void onClose(Channel *);

void onClientConnect(Channel *, TcpClient *);
void onClientRead(Channel *channel, const char *, int , TcpClient *);
void onClientWrite(Channel *channel, const char *, int , TcpClient *);

void print();
 
int port = 12345;
const char *host = "127.0.0.1";

long cliRecived = 0;
long cliSent = 0;
long servRecived = 0;
long servSent = 0;

void input();

TcpClient *clientPtr = NULL;
Channel *cliChannel = NULL;

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

  TcpServer serv1(port, host);

  serv1.setOnConnectFunc(boost::bind(&onConnect, _1, &serv1));
  serv1.setOnCloseFunc(boost::bind(&onClose, _1));
  serv1.setOnReadFunc(boost::bind(&onRead, _1, _2, _3, &serv1));
  serv1.setOnWriteFunc(boost::bind(&onWrite, _1, _2, _3, &serv1));
  serv1.start(&serv1Loop);

  TcpClient client(&cliLoop);
  client.setOnConnectFunc(boost::bind(&onClientConnect, _1, &client));
  client.setOnReadFunc(boost::bind(&onClientRead, _1, _2, _3, &client));
  client.setOnWriteFunc(boost::bind(&onClientWrite, _1, _2, _3, &client));
  client.start();

  Eventloop<Epoll> timerLoop; 
  Timer timer;
  timer.start(&timerLoop);
  timer.schedule(boost::bind(&print), 5000l, 3000l, -1);

  clientPtr = &client;
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
        // LOG_INFO << client_num;
      }
      else if (::strcmp(string, "connect\n") == 0)
      {
        clientPtr->connect(12345);
      }
      else if (::strcmp(string, "send\n") == 0)
      {
        clientPtr->send(cliChannel , "hello", 5);
      }
    }
  }

}

void onConnect(Channel *channel, TcpServer * serv)
{
  // client_num++;
  // LOG_INFO << "connect fd: " << channel->getFd();
  serv->send(channel , "hello", 5);
}
void onRead(Channel *channel, const char *buf, int len, TcpServer *serv)
{
  servRecived+=len;
  // serv->send(channel , "hello", 5);
}
void onWrite(Channel *channel, const char *msg, int len, TcpServer *serv)
{
  servSent+=len;
}
void onClose(Channel *channel)
{
}

void onClientConnect(Channel *channel, TcpClient *cli)
{
  // cli->send(channel , "hello", 5);
  cliChannel = channel;
  LOG_INFO << "client connect fd: " << channel->getFd();
}
void onClientRead(Channel *channel, const char *buf, int len, TcpClient *cli)
{
  LOG_INFO << "client read fd: " << channel->getFd()
        << ", buf: " << buf
        << ", len: " << len;
  cliRecived+=len;
  // cli->send(channel , "hello", 5);
}
void onClientWrite(Channel *channel, const char *msg, int len, TcpClient *cli)
{
  cliSent+=len;
}

void print()
{ 

  LOG_INFO_R << " cliRecived: " << cliRecived
  << ", cliSent: "<<cliSent
  << ", servRecived: "<<servRecived
  << ", servSent: " <<servSent;

}
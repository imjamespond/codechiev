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

typedef Channel::ChannelPtr ChannelPtr;

void onConnect(const ChannelPtr &, TcpServer *);
void onRead(const ChannelPtr &, const char *, int, TcpServer *);
void onWrite(const ChannelPtr &, const char *, int, TcpServer *);
void onClose(const ChannelPtr &);

void onClientConnect(const ChannelPtr &, TcpClient *);
void onClientCompleteWrite(const ChannelPtr &, TcpClient *);
void onClientRead(const ChannelPtr &, const char *, int , TcpClient *);
void onClientWrite(const ChannelPtr &, const char *, int , TcpClient *);

void print();
 
int port = 12345;
const char *host = "127.0.0.1";

long cliRecived = 0;
long cliSent = 0;
long servRecived = 0;
long servSent = 0;
int sendSize = 1024 * 1024 * 32;

void input();

TcpClient *clientPtr = NULL; 

void gen_random(char *s, const int len) {
  /* initialize random seed: */
  srand (time(NULL));

  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

  for (int i = 0; i < len; ++i) {
      s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  s[len] = 0;
}

char randStr[1024];

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

  gen_random(randStr, sizeof randStr);

  LOG_INFO << "host: " << host << ", port: " << port;

  Eventloop<Epoll> serv1Loop; 
  Eventloop<Epoll> cliLoop;

  // TcpServer serv1(port, host);

  // serv1.setOnConnectFunc(boost::bind(&onConnect, _1, &serv1));
  // serv1.setOnCloseFunc(boost::bind(&onClose, _1));
  // serv1.setOnReadFunc(boost::bind(&onRead, _1, _2, _3, &serv1));
  // serv1.setOnWriteFunc(boost::bind(&onWrite, _1, _2, _3, &serv1));
  // serv1.start(&serv1Loop);

  TcpClient client(&cliLoop);
  client.setOnConnectFunc(boost::bind(&onClientConnect, _1, &client));
  client.setOnReadFunc(boost::bind(&onClientRead, _1, _2, _3, &client));
  client.setOnWriteFunc(boost::bind(&onClientWrite, _1, _2, _3, &client));
  client.setOnCompleteWriteFunc(boost::bind(&onClientCompleteWrite, _1, &client));
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
        clientPtr->connect(port, host);
      } 
    }
  }

}

void onConnect(const ChannelPtr &channel, TcpServer * serv)
{
  // client_num++;
  // LOG_INFO << "connect fd: " << channel->getFd();
  serv->send(channel , randStr, sizeof randStr);
}
void onRead(const ChannelPtr &channel, const char *buf, int len, TcpServer *serv)
{
  servRecived+=len;
  serv->send(channel , buf, len);
}
void onWrite(const ChannelPtr &channel, const char *msg, int len, TcpServer *serv)
{
  servSent+=len;
}
void onClose(const ChannelPtr &channel)
{
}

void onClientConnect(const ChannelPtr &channel, TcpClient *cli)
{
  LOG_INFO << "client connect fd: " << channel->getFd();
  cli->send(channel, randStr, sizeof randStr);
}
void onClientRead(const ChannelPtr &channel, const char *buf, int len, TcpClient *cli)
{
  // LOG_INFO << "client read fd: " << channel->getFd()
  //       << ", buf: " << buf
  //       << ", len: " << len;
  cliRecived+=len;
  // cli->send(channel , buf, len);
}
void onClientWrite(const ChannelPtr &channel, const char *msg, int len, TcpClient *cli)
{
  cliSent+=len;
}
void onClientCompleteWrite(const ChannelPtr &channel, TcpClient *cli)
{
  if ((sendSize -= static_cast<int>(sizeof randStr)) > 0)
  {
    cli->send(channel, randStr, sizeof randStr);
    LOG_INFO << sendSize;
  }

}

void print()
{ 

  LOG_INFO_R << " cliRecived: " << (cliRecived>>20) <<"mb,"
  << "cliSent: "<< (cliSent>>20) << "mb,"
  << "servRecived: "<< (servRecived>>20) << "mb,"
  << "servSent: " << (servSent>>20) << "mb,";

}
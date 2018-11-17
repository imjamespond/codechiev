#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <errno.h>
#include <sys/epoll.h>
// #include <signal.h>
#include <boost/bind.hpp>

#include <network/TcpServer.hpp>
#include <network/TcpClient.hpp>

#include <base/Time.hpp>
#include <base/Logger.hpp>
#include <base/Keyboard.hpp>

using namespace codechiev::net;
using namespace codechiev::base;

void onConnect(Channel *, TcpServer *);
void onRead(Channel *, const char *, int, TcpServer *);
void onWrite(Channel *, const char *, int, TcpServer *);
void onClose(Channel *);

void onClientConnect(Channel *);
void onClientRead(Channel *, const char *, int, TcpClient *);

int client_num = 0;
int client_total = 1;

TcpClient *clientPtr;

void input();

int main(int num, const char **args)
{
  if (num > 1)
  {
    client_total = ::atoi(args[1]);
    LOG_INFO << client_total;
  }
  
  // struct sigaction st[] = {SIG_IGN};
  // sigaction(SIGPIPE, st, NULL);

  Eventloop<Epoll> cliLoop;
  TcpClient client(&cliLoop);

  client.setOnConnectFunc(boost::bind(&onClientConnect, _1));
  client.setOnReadFunc(boost::bind(&onClientRead, _1, _2, _3, &client));
  client.start();

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
        LOG_INFO << client_num;
      }
      else if (::strcmp(string, "connect\n") == 0)
      {
        for (int i = 0; i < client_total; ++i)
        {
          clientPtr->connect(12345, "127.0.0.1");
        }
      }
    }
  }

}

void onClientConnect(Channel *channel)
{
  // LOG_INFO << "connect fd: " << channel->getFd();
}

void onClientRead(Channel * channel, const char *buf, int len, TcpClient *cli)
{
  LOG_INFO << "read fd: " << channel->getFd()
            << ", buf: " << buf
            << ", len: " << len;
}
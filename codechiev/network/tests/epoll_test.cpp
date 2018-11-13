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

void onConnect(Channel *);
void onRead(Channel *, const char *, int);
void onWrite(Channel *, const char *, int);
void onClose(Channel *);
void epollHandler(Epoll *, Channel *, Channel *);

TcpServer endpoint;
TcpClient client;
int client_num = 0;
int client_total = 1;

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

  endpoint.setOnConnectFunc(boost::bind(&onConnect, _1));
  endpoint.setOnReadFunc(boost::bind(&onRead, _1, _2, _3));
  endpoint.setOnWriteFunc(boost::bind(&onWrite, _1, _2, _3));
  endpoint.setOnCloseFunc(boost::bind(&onClose, _1));
  endpoint.start(12345);

  client.start();

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
void onRead(Channel *channel, const char *buf, int len)
{
  LOG_INFO << "read fd: " << channel->getFd()
            << ", buf: " << buf
            << ", len: " << len;
  // Must not shut down when channel is writable, otherwise will received SIGPIPE, broken pipe?
  // https://stackoverflow.com/questions/18935446/program-received-signal-sigpipe-broken-pipe
  if (strcmp(buf, "close\n") == 0)
  {
    endpoint.shutdown(channel);
  }
  endpoint.send(channel, buf, len);
}
void onWrite(Channel *channel, const char *msg, int len)
{
  if (strcmp(msg, "write-close\n") == 0)
  {
    endpoint.shutdown(channel);
  }
  LOG_INFO << "write fd: " << channel->getFd();
}
void onClose(Channel *channel)
{
  LOG_INFO << "close fd: " << channel->getFd();
}
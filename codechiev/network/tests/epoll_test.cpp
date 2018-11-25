#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <errno.h>
#include <sys/epoll.h>
#include <signal.h>
#include <boost/bind.hpp>
#include <boost/unordered_map.hpp>

#include <network/TcpServer.hpp>
#include <network/TcpClient.hpp>

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
void onClientRead(const ChannelPtr &, const char *, int, TcpClient *);
 
int client_total = 1;

TcpClient *clientPtr;
TcpServer *serverPtr; 

typedef boost::unordered_map<int, ChannelPtr> channel_map;
channel_map channels;
Mutex channelsMutex;

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

  Eventloop<Epoll> serv1Loop1;// each server could have more than one loop
  Eventloop<Epoll> serv1Loop2;
  Eventloop<Epoll> serv2Loop1;
  Eventloop<Epoll> serv2Loop2;
  Eventloop<Epoll> cliLoop;

  TcpServer serv1(12345);
  // TcpServer serv2(12345, "192.168.0.254");
  TcpClient client(&cliLoop);

  serv1.setOnConnectFunc(boost::bind(&onConnect, _1, &serv1));
  serv1.setOnReadFunc(boost::bind(&onRead, _1, _2, _3, &serv1));
  serv1.setOnWriteFunc(boost::bind(&onWrite, _1, _2, _3, &serv1));
  serv1.setOnCloseFunc(boost::bind(&onClose, _1));
  serv1.start(&serv1Loop1, false);
  serv1.start(&serv1Loop2);

  // serv2.setOnConnectFunc(boost::bind(&onConnect, _1, &serv2));
  // serv2.setOnReadFunc(boost::bind(&onRead, _1, _2, _3, &serv2));
  // serv2.setOnWriteFunc(boost::bind(&onWrite, _1, _2, _3, &serv2));
  // serv2.start(&serv2Loop1);
  // serv2.start(&serv2Loop2);

  client.setOnConnectFunc(boost::bind(&onClientConnect, _1, &client));
  client.setOnReadFunc(boost::bind(&onClientRead, _1, _2, _3, &client));
  client.start();

  clientPtr = &client;
  serverPtr = &serv1;
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
        LOG_INFO << channels.size();
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

void onConnect(const ChannelPtr &channel, TcpServer *serv)
{ 
  // serv->send(channel , "hello", 5);
  {
    MutexGuard lock(&channelsMutex);
    channels[channel->getFd()] = channel;
  }

  LOG_INFO << "connect fd: " << channel->getFd();
}
void onRead(const ChannelPtr &channel, const char *buf, int len, TcpServer * serv)
{
  LOG_INFO << "read fd: " << channel->getFd()
            << ", buf: " << buf
            << ", len: " << len;
  // Must not shut down when channel is writable, otherwise will received SIGPIPE, broken pipe?
  // https://stackoverflow.com/questions/18935446/program-received-signal-sigpipe-broken-pipe

  // serv->send(channel, buf, len);
  {
    MutexGuard lock(&channelsMutex);
    channel_map::iterator it = channels.begin();
    while (it != channels.end())
    {
      const ChannelPtr &_channel = it->second;
      if (strcmp(buf, "close\n") == 0)
      {
        serv->shutdown(_channel);
      }
      else
        serv->send(_channel , buf, len);

      ++it;
    } 
  }
}
void onWrite(const ChannelPtr &channel, const char *msg, int len, TcpServer *serv)
{
  if (strcmp(msg, "write-close\n") == 0)
  {
    serv->shutdown(channel);
  }
  LOG_INFO << "write fd: " << channel->getFd();
}
void onClose(const ChannelPtr &channel)
{
  LOG_INFO << "close fd: " << channel->getFd();

  {
    MutexGuard lock(&channelsMutex);
    channels.erase(channel->getFd());
  }
}

void onClientConnect(const ChannelPtr &channel, TcpClient *cli)
{
  LOG_INFO << "connect fd: " << channel->getFd();
}

void onClientRead(const ChannelPtr & channel, const char *buf, int len, TcpClient *cli)
{
  LOG_INFO << "read fd: " << channel->getFd()
            << ", buf: " << buf
            << ", len: " << len;
}
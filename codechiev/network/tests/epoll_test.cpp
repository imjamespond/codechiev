#include <network/Eventloop.hpp>
#include <network/Epoll.hpp>
#include <network/TcpServer.hpp>
#include <network/TcpClient.hpp>
#include <base/Logger.hpp>
#include <base/Singleton.hpp>
#include <base/Time.hpp>
#include <vector>
#include <signal.h>

using namespace codechiev::base;
using namespace codechiev::net;

typedef Channel::ChannelPtr ChannelPtr;

const char *port = "12345";
const char *host = "127.0.0.1";

void onConnect(const ChannelPtr &, TcpServer *);
void onClose(const ChannelPtr &);
void onRead(const ChannelPtr &, const char *, int);
void onWrite(const ChannelPtr &, const char *, int);

void onCliConnect(const ChannelPtr &, TcpClient *);
void onCliClose(const ChannelPtr &);
void onCliRead(const ChannelPtr &, const char *, int);
void onCliWrite(const ChannelPtr &, const char *, int);

typedef std::vector<ChannelPtr> ChannelList; 
ChannelList cliList; 
ChannelList svrList; 

int totalWrite(0);
int totalRead(0);
int totalConnected(0);
int totalClosed(0);

int totalCliWrite(0);
int totalCliRead(0);
int totalCliConnected(0);
int totalCliClosed(0);

int main(int num, const char **args)
{ 
  if (num > 1)
  {
    host = args[1];
  }
  if (num > 2)
  {
    port = args[2]; 
  }

  struct sigaction st[] = {SIG_IGN};
  sigaction(SIGPIPE, st, NULL);

  LOG_INFO << "host: " << host << ", port: " << port;

  TcpServer serv1(port, host); 
  serv1.onConnect=(boost::bind(&onConnect, _1, &serv1));
  serv1.onClose=(boost::bind(&onClose, _1));
  serv1.onRead=boost::bind(&onRead, _1, _2, _3);
  serv1.onWrite=boost::bind(&onWrite, _1, _2, _3);

  TcpClient client = TcpClient();
  client.onConnect=(boost::bind(&onCliConnect, _1, &client));
  client.onClose=(boost::bind(&onCliClose, _1));
  client.onRead=boost::bind(&onCliRead, _1, _2, _3);
  client.onWrite=boost::bind(&onCliWrite, _1, _2, _3);

  {
    Channel::Loop loop;

    serv1.init(&loop);
    client.init(&loop);

    loop.loop();

    for (int i(0); i<10; i++ )
    { 
      Time::SleepMillis(10l);
      client.connect(port, host);
    }

    Time::SleepMillis(2000l);//wait for all clients connected

    // set server to hup and client send
    ChannelList::iterator svrIt;
    for (svrIt = svrList.begin(); svrIt != svrList.end(); svrIt++ )
    { 
      ChannelPtr channel = *svrIt;
    }

    ChannelList::iterator cliIt;
    for (cliIt = cliList.begin(); cliIt != cliList.end(); cliIt++ )
    { 
      ChannelPtr channel = *cliIt;
      const char *msg = "testtesttesttesttesttesttesttesttesttesttesttesttesttesttest";
      client.send(channel , msg, ::strlen(msg) );
    }

    Time::SleepMillis(2000l);

    for (cliIt = cliList.begin(); cliIt != cliList.end(); cliIt++ )
    { 
      ChannelPtr channel = *cliIt;
      channel->shutdown();
    }
  }

}

void onConnect(const ChannelPtr &channel, TcpServer *serv)
{ 
  svrList.push_back(channel);
  serv->send(channel , "serv->send", sizeof "serv->send");
  LOG_INFO << "connect fd: " << channel->getFd() << ", " << ++totalConnected;
}
void onClose(const ChannelPtr &channel)
{
  LOG_INFO << "close fd: " << channel->getFd()<< ", " << ++totalClosed;
}
void onRead(const ChannelPtr &channel, const char *msg, int len)
{
  totalRead += len;
  LOG_INFO << "fd: " << channel->getFd() << ", total read: " << totalRead;
}
void onWrite(const ChannelPtr &channel, const char *msg, int len)
{
  totalWrite += len;
  LOG_INFO << "fd: " << channel->getFd() << ", total write: " << totalWrite;
}


void onCliConnect(const ChannelPtr &channel, TcpClient *cli)
{
  cliList.push_back(channel);
  cli->send(channel , "cli->send", sizeof "cli->send");
  LOG_INFO << "connect fd: " << channel->getFd() << ", count:" << cliList.size();
}
void onCliClose(const ChannelPtr &channel)
{
  LOG_INFO << "close fd: " << channel->getFd() << ", count:" << cliList.size();
}
void onCliRead(const ChannelPtr &channel, const char *msg, int len)
{
  totalCliRead += len;
  LOG_INFO << "fd: " << channel->getFd() << ", total cli read: " << totalCliRead;
}
void onCliWrite(const ChannelPtr &channel, const char *msg, int len)
{
  totalCliWrite += len;
  LOG_INFO << "fd: " << channel->getFd() << ", total cli write: " << totalCliWrite;
}
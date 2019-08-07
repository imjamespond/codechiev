#include <network/Eventloop.hpp>
#include <network/Epoll.hpp>
#include <network/TcpServer.hpp>
#include <network/TcpClient.hpp>
#include <base/Logger.hpp>
#include <base/Singleton.hpp>
#include <base/Time.hpp>
#include <vector>

using namespace codechiev::base;
using namespace codechiev::net;

typedef Channel::ChannelPtr ChannelPtr;

const char *port = "12345";
const char *host = "127.0.0.1";

void onConnect(const ChannelPtr &, TcpServer *);
void onClose(const ChannelPtr &);

void onCliConnect(const ChannelPtr &, TcpClient *);
void onCliClose(const ChannelPtr &);

typedef std::vector<ChannelPtr> ChannelList; 
ChannelList cliList; 

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

  LOG_INFO << "host: " << host << ", port: " << port;

  TcpServer serv1(port, host, true); //avoid listen channel destroy before loop joined
  serv1.setOnConnect(boost::bind(&onConnect, _1, &serv1));
  serv1.setOnClose(boost::bind(&onClose, _1));

  TcpClient client(true);
  client.setOnConnect(boost::bind(&onCliConnect, _1, &client));
  client.setOnClose(boost::bind(&onCliClose, _1));

  {
    Channel::Loop loop;

    serv1.init(&loop);
    client.init(&loop);

    loop.loop();

    for (int i(0); i<10; i++ )
    { 
      client.connect(port, host);
      Time::SleepMillis(10l);
    }

    Time::SleepMillis(1000l);

    ChannelList::iterator cliIt;
    for (cliIt = cliList.begin(); cliIt != cliList.end(); cliIt++ )
    { 
      ChannelPtr cli = *cliIt;
      cli->shutdown();
    }
  }

}

void onConnect(const ChannelPtr &channel, TcpServer *serv)
{ 
  serv->send(channel , "serv->send", sizeof "serv->send");

  LOG_INFO << "connect fd: " << channel->getFd();
}

void onClose(const ChannelPtr &channel)
{
  LOG_INFO << "close fd: " << channel->getFd();
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
#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <errno.h>
#include <sys/epoll.h>
#include <signal.h>

#include <boost/bind.hpp>
#include <boost/unordered_map.hpp>

#include <network/TcpServer.hpp>
#include <network/TcpClient.hpp>
#include <network/Timer.hpp>

#include <base/Time.hpp>
#include <base/Logger.hpp>
#include <base/Keyboard.hpp>
#include <base/UUID.hpp>

#include "./TunnelChannel.hpp"

using namespace codechiev::net;
using namespace codechiev::base;

void onConnect(Channel *, TcpServer *);
void onRead(Channel *, const char *, int, TcpServer *);
void onWrite(Channel *, const char *, int, TcpServer *);
void onClose(Channel *);

void onClientConnect(Channel *, TcpClient *);
void onClientRead(Channel *channel, const char *, int , TcpClient *);
void onClientWrite(Channel *channel, const char *, int , TcpClient *);
void onClientClose(Channel *);

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

typedef boost::unordered_map<UUID::uuid_t, TunnelChannel *> uuid_map;
uuid_map serverChannels;
uuid_map clientChannels;

TunnelChannel* createServChannel(int);
TunnelChannel* createCliChannel(int, const UUID::uuid_t&);

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

  struct sigaction st[] = {SIG_IGN};
  sigaction(SIGPIPE, st, NULL);


  // uuid_map::iterator it = uuidMap.begin();
  // while (it != uuidMap.end())
  // {
  //   const boost::uuids::uuid &session = it->first;
  //   int i = it->second;
  //   std::string session_str = boost::lexical_cast<std::string>(session);
  //   LOG_INFO << session_str << "," <<i;
  //   ++it;
  // }



  // LOG_INFO << "host: " << host << ", port: " << port;

  LOG_INFO << "host: " << host << ", port: " << port;

  // FIXME use shared_ptr
  Eventloop<Epoll> serv1Loop; 
  Eventloop<Epoll> cliLoop;

  TcpServer serv1(1080);
  TcpClient client(&cliLoop);
  clientPtr = &client;

  serv1.setCreateChannel(boost::bind(&createServChannel, _1));
  serv1.setOnConnectFunc(boost::bind(&onConnect, _1, &serv1));
  serv1.setOnCloseFunc(boost::bind(&onClose, _1));
  serv1.setOnReadFunc(boost::bind(&onRead, _1, _2, _3, &serv1));
  serv1.setOnWriteFunc(boost::bind(&onWrite, _1, _2, _3, &serv1));
  serv1.start(&serv1Loop);

  client.setOnConnectFunc(boost::bind(&onClientConnect, _1, &client));
  client.setOnCloseFunc(boost::bind(&onClientClose, _1));
  client.setOnReadFunc(boost::bind(&onClientRead, _1, _2, _3, &client));
  client.setOnWriteFunc(boost::bind(&onClientWrite, _1, _2, _3, &client));
  client.start();

  Eventloop<Epoll> timerLoop; 
  Timer timer;
  timer.start(&timerLoop);
  timer.schedule(boost::bind(&print), 5000l, 3000l, -1);

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
    }
  }

}

void onConnect(Channel *channel, TcpServer * serv)
{
  // client_num++;
  TunnelChannel *_channel = static_cast<TunnelChannel *>(channel);
  serverChannels[_channel->getSession()] = _channel;
  clientPtr->connect(port, host);
  LOG_INFO << "connect fd: " << channel->getFd();
}
void onRead(Channel *channel, const char *buf, int len, TcpServer *serv)
{
  LOG_INFO << "read fd: " << channel->getFd()
        << ", buf: " << buf
        << ", len: " << len;  
  servRecived+=len;
  serv->send(channel , buf, len);//send to tunnel
}
void onWrite(Channel *channel, const char *msg, int len, TcpServer *serv)
{
  servSent+=len;
}
void onClose(Channel *channel)
{
}

void onClientConnect(Channel *channel, TcpClient *endpoint)
{
  // client_num++;
  LOG_INFO << "connect fd: " << channel->getFd();
}
void onClientRead(Channel *channel, const char *buf, int len, TcpClient *endpoint)
{
  LOG_INFO << "read fd: " << channel->getFd()
           << ", buf: " << buf
           << ", len: " << len;
  servRecived += len;
  endpoint->send(channel, buf, len); //send to tunnel
}
void onClientWrite(Channel *channel, const char *msg, int len, TcpClient *endpoint)
{
  servSent += len;
}
void onClientClose(Channel *channel)
{
}

void print()
{ 

  LOG_INFO_R << " cliRecived: " << (cliRecived>>20) <<"mb,"
  << "cliSent: "<< (cliSent>>20) << "mb,"
  << "servRecived: "<< (servRecived>>10) << "kb,"
  << "servSent: " << (servSent>>10) << "kb,";

}
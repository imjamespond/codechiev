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
#include <network/socket.h>

#include <base/Time.hpp>
#include <base/Logger.hpp>
#include <base/Keyboard.hpp>
#include <base/UUID.hpp>

#include "./TunnelChannel.hpp"

using namespace codechiev::net;
using namespace codechiev::base;

void onConnect(Channel *, TcpServer *, TcpClient *);
void onRead(Channel *, const char *, int, TcpServer *, TcpClient *);
void onWrite(Channel *, const char *, int, TcpServer *);
void onClose(Channel *);

void onClientConnect(Channel *, TcpClient *);
void onClientRead(Channel *channel, const char *, int, TcpClient *, TcpServer *serv);
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
Mutex serverMutex;
Mutex clientMutex;

TunnelChannel* createTunnelChannel(int);

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

  LOG_INFO << "host: " << host << ", port: " << port;

  // FIXME use shared_ptr
  Eventloop<Epoll> serv1Loop; 
  Eventloop<Epoll> cliLoop;

  TcpServer serv1(1080);
  TcpClient client(&cliLoop);
  // clientPtr = &client;

  serv1.setCreateChannel(boost::bind(&createTunnelChannel, _1));
  serv1.setOnConnectFunc(boost::bind(&onConnect, _1, &serv1, &client));
  serv1.setOnCloseFunc(boost::bind(&onClose, _1));
  serv1.setOnReadFunc(boost::bind(&onRead, _1, _2, _3, &serv1, &client));
  serv1.setOnWriteFunc(boost::bind(&onWrite, _1, _2, _3, &serv1));
  serv1.start(&serv1Loop);

  client.setCreateChannel(boost::bind(&createTunnelChannel, _1));
  client.setOnConnectFunc(boost::bind(&onClientConnect, _1, &client));
  client.setOnCloseFunc(boost::bind(&onClientClose, _1));
  client.setOnReadFunc(boost::bind(&onClientRead, _1, _2, _3, &client, &serv1));
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

void onConnect(Channel *channel, TcpServer *serv, TcpClient *cli)
{
  // client_num++;
  TunnelChannel *_channel = static_cast<TunnelChannel *>(channel);
  int conn_sock = Connect(port, host);
  Channel *conn = new TunnelChannel(conn_sock);
  cli->connect(conn);
  TunnelChannel *_conn = static_cast<TunnelChannel *>(conn);
  _conn->session = _channel->session;
  {
    MutexGuard lock(&serverMutex);
    serverChannels[_channel->session] = _channel;
  }
  {
    MutexGuard lock(&clientMutex);
    clientChannels[_channel->session] = _conn;
  }
  
  LOG_INFO << "connect fd: " << channel->getFd();
}
void onRead(Channel *channel, const char *buf, int len, TcpServer *serv, TcpClient *cli)
{
  LOG_INFO << "read fd: " << channel->getFd()
        << ", buf: " << buf
        << ", len: " << len;
  servRecived += len;

  TunnelChannel *_channel = static_cast<TunnelChannel *>(channel);
  {
    MutexGuard lock(&clientMutex);
    TunnelChannel *_conn = clientChannels[_channel->session];
    if (_conn)
      cli->send(_conn, buf, len); //send to tunnel
  }

}
void onWrite(Channel *channel, const char *msg, int len, TcpServer *serv)
{
  servSent += len;
}
void onClose(Channel *channel)
{
  TunnelChannel *_channel = static_cast<TunnelChannel *>(channel);
  {
    MutexGuard lock(&serverMutex);
    serverChannels.erase(_channel->session);
  }
  {
    MutexGuard lock(&clientMutex);
    uuid_map::iterator connIt = clientChannels.find(_channel->session);
    if (connIt != clientChannels.end())
    {
      connIt->second->shutdown();
      clientChannels.erase(connIt);
    } 
  }
}

void onClientConnect(Channel *channel, TcpClient *endpoint)
{
  // client_num++;
  LOG_INFO << "connect fd: " << channel->getFd();
  endpoint->send(channel, "", 0);
}
void onClientRead(Channel *channel, const char *buf, int len, TcpClient *endpoint, TcpServer *serv)
{
  LOG_INFO << "read fd: " << channel->getFd()
           << ", buf: " << buf
           << ", len: " << len;
  cliRecived += len;

  TunnelChannel *_channel = static_cast<TunnelChannel *>(channel);
  {
    MutexGuard lock(&serverMutex);
    TunnelChannel *_conn = serverChannels[_channel->session];
    if (_conn)
      serv->send(_conn, buf, len); //send to tunnel
  }

}
void onClientWrite(Channel *channel, const char *msg, int len, TcpClient *endpoint)
{
  cliSent += len;
}
void onClientClose(Channel *channel)
{
  TunnelChannel *_channel = static_cast<TunnelChannel *>(channel);
  {
    MutexGuard lock(&serverMutex);
    uuid_map::iterator connIt = serverChannels.find(_channel->session);
    if (connIt != serverChannels.end())
    {
      connIt->second->shutdown();
      serverChannels.erase(connIt);
    } 
  }
  {
    MutexGuard lock(&clientMutex);
    clientChannels.erase(_channel->session);
  }
}

void print()
{ 

  LOG_INFO_R << " cliRecived: " << (cliRecived>>0) <<","
  << "cliSent: "<< (cliSent>>0) << ","
  << "servRecived: "<< (servRecived>>0) << ","
  << "servSent: " << (servSent>>0) << "";

}

TunnelChannel *createTunnelChannel(int sockfd)
{
  return new TunnelChannel(sockfd);
}
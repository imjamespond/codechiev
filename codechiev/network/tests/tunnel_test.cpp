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

typedef Channel::ChannelPtr ChannelPtr;

void onConnect(const ChannelPtr &, TcpServer *, TcpClient *);
void onRead(const ChannelPtr &, const char *, int, TcpServer *, TcpClient *);
void onWrite(const ChannelPtr &, const char *, int, TcpServer *);
void onClose(const ChannelPtr &, TcpServer *, TcpClient *);

void onClientConnect(const ChannelPtr &, TcpClient *);
void onClientRead(const ChannelPtr &channel, const char *, int, TcpClient *, TcpServer *serv);
void onClientWrite(const ChannelPtr &channel, const char *, int, TcpClient *);
void onClientClose(const ChannelPtr &, TcpServer *, TcpClient *);

void print();

int port = 12345;
const char *host = "127.0.0.1";

long cliRecived = 0;
long cliSent = 0;
long servRecived = 0;
long servSent = 0;

void input();

// typedef boost::unordered_map<UUID::uuid_t, TunnelChannel *> uuid_map;
// uuid_map serverChannels;
// uuid_map clientChannels;
// Mutex serverMutex;
// Mutex clientMutex;

TunnelChannel *createTunnelChannel(int);

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
  serv1.setOnCloseFunc(boost::bind(&onClose, _1, &serv1, &client));
  serv1.setOnReadFunc(boost::bind(&onRead, _1, _2, _3, &serv1, &client));
  serv1.setOnWriteFunc(boost::bind(&onWrite, _1, _2, _3, &serv1));
  serv1.start(&serv1Loop);

  client.setCreateChannel(boost::bind(&createTunnelChannel, _1));
  client.setOnConnectFunc(boost::bind(&onClientConnect, _1, &client));
  client.setOnCloseFunc(boost::bind(&onClientClose, _1, &serv1, &client));
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

void onConnect(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
{
  // client_num++;
  TunnelChannel *serv_conn = static_cast<TunnelChannel *>(channel.get());
  int conn_sock = Connect(port, host);
  TunnelChannel *cli_conn = createTunnelChannel(conn_sock);
  serv_conn->tunnel = cli_conn->ptr;
  cli_conn->tunnel = serv_conn->ptr;

  cli->connect(cli_conn);

  LOG_INFO << "connect fd: " << channel->getFd();
}
void onRead(const ChannelPtr &channel, const char *buf, int len, TcpServer *serv, TcpClient *cli)
{
  // LOG_INFO << "read fd: " << channel->getFd()
  //       << ", buf: " << buf
  //       << ", len: " << len;
  servRecived += len;

  TunnelChannel *serv_conn = static_cast<TunnelChannel *>(channel.get());

  if (ChannelPtr cli_conn = serv_conn->tunnel.lock())
  {
    cli->send(cli_conn, buf, len); //send to tunnel
  }
}
void onWrite(const ChannelPtr &channel, const char *msg, int len, TcpServer *serv)
{
  servSent += len;
}
void onClose(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
{
  TunnelChannel *serv_conn = static_cast<TunnelChannel *>(channel.get());
  if (ChannelPtr cli_conn = serv_conn->tunnel.lock())
  {
    cli->shutdown(cli_conn);
  }
}

void onClientConnect(const ChannelPtr &channel, TcpClient *endpoint)
{
  // client_num++;
  LOG_INFO << "connect fd: " << channel->getFd();
  endpoint->send(channel, "", 0);
}
void onClientRead(const ChannelPtr &channel, const char *buf, int len, TcpClient *endpoint, TcpServer *serv)
{
  // LOG_INFO << "read fd: " << channel->getFd()
  //          << ", buf: " << buf
  //          << ", len: " << len;
  cliRecived += len;

  TunnelChannel *cli_conn = static_cast<TunnelChannel *>(channel.get());
  if (ChannelPtr serv_conn = cli_conn->tunnel.lock())
  {
    serv->send(serv_conn, buf, len); //send to tunnel
  }
}
void onClientWrite(const ChannelPtr &channel, const char *msg, int len, TcpClient *endpoint)
{
  cliSent += len;
}
void onClientClose(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
{
  TunnelChannel *cli_conn = static_cast<TunnelChannel *>(channel.get());
  if (ChannelPtr serv_conn = cli_conn->tunnel.lock())
  {
    serv->shutdown(serv_conn);
  }
}

void print()
{

  LOG_INFO_R << "cliRecived: " << (cliRecived >> 0) << ","
             << "cliSent: " << (cliSent >> 0) << ","
             << "servRecived: " << (servRecived >> 0) << ","
             << "servSent: " << (servSent >> 0) << "";
}

TunnelChannel *createTunnelChannel(int sockfd)
{
  return new TunnelChannel(sockfd);
}
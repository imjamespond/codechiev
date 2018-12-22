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
void onPartialRead(const ChannelPtr &, const char *, int, TcpServer *, TcpClient *);
void onPartialWrite(const ChannelPtr &, const char *, int, TcpServer *);
void onRead(const ChannelPtr &channel,  TcpServer *, TcpClient *);
void onWrite(const ChannelPtr &channel, TcpClient *);
void onClose(const ChannelPtr &, TcpServer *, TcpClient *);

void onClientConnect(const ChannelPtr &, TcpClient *);
void onClientPartialRead(const ChannelPtr &channel, const char *, int, TcpClient *, TcpServer *);
void onClientPartialWrite(const ChannelPtr &channel, const char *, int, TcpClient *);
void onClientRead(const ChannelPtr &channel,  TcpClient *, TcpServer *);
void onClientWrite(const ChannelPtr &channel, TcpServer *);
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

  TcpServer serv1(1080, "0.0.0.0", false); //level triggered
  TcpClient client(&cliLoop, false);       //level triggered
  // clientPtr = &client;

  serv1.setCreateChannel(boost::bind(&createTunnelChannel, _1));
  serv1.setOnConnectFunc(boost::bind(&onConnect, _1, &serv1, &client));
  serv1.setOnCloseFunc(boost::bind(&onClose, _1, &serv1, &client));
  serv1.setOnPartialReadFunc(boost::bind(&onPartialRead, _1, _2, _3, &serv1, &client));
  serv1.setOnPartialWriteFunc(boost::bind(&onPartialWrite, _1, _2, _3, &serv1));
  serv1.setOnReadFunc(boost::bind(&onRead, _1, &serv1, &client));
  serv1.setOnWriteFunc(boost::bind(&onWrite, _1, &client));
  serv1.start(&serv1Loop);

  client.setCreateChannel(boost::bind(&createTunnelChannel, _1));
  client.setOnConnectFunc(boost::bind(&onClientConnect, _1, &client));
  client.setOnCloseFunc(boost::bind(&onClientClose, _1, &serv1, &client));
  client.setOnPartialReadFunc(boost::bind(&onClientPartialRead, _1, _2, _3, &client, &serv1));
  client.setOnPartialWriteFunc(boost::bind(&onClientPartialWrite, _1, _2, _3, &client));
  client.setOnReadFunc(boost::bind(&onClientRead, _1, &client, &serv1));
  client.setOnWriteFunc(boost::bind(&onClientWrite, _1, &serv1));
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

  // LOG_INFO << "connect fd: " << channel->getFd();
}
void onPartialRead(const ChannelPtr &channel, const char *buf, int len, TcpServer *serv, TcpClient *cli)
{
  // LOG_INFO << "read fd: " << channel->getFd()
  //       // << ", buf: " << buf
  //       << ", len: " << len;
  servRecived += len;

  TunnelChannel *conn = static_cast<TunnelChannel *>(channel.get());

  if (ChannelPtr tunnel = conn->tunnel.lock())
  {
    if (cli->write(tunnel, buf, len) < 0) 
    {
      LOG_DEBUG << "flush: " << tunnel->getFd();
      serv->enableRead(channel, false); //make sure stop read before send 
      cli->flush(tunnel);
    }
  }
}
void onRead(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
{
  TunnelChannel *conn = static_cast<TunnelChannel *>(channel.get());

  if (ChannelPtr tunnel = conn->tunnel.lock())
  {
    LOG_DEBUG << "flush: " << tunnel->getFd();
    serv->enableRead(channel, false);
    cli->flush(tunnel); //send to tunnel
  }
}
void onPartialWrite(const ChannelPtr &channel, const char *msg, int len, TcpServer *serv)
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
void onWrite(const ChannelPtr &channel, TcpClient *cli)
{
  TunnelChannel *conn = static_cast<TunnelChannel *>(channel.get());

  if (ChannelPtr tunnel = conn->tunnel.lock())
  {
    cli->enableRead(tunnel); 
  }
}

void onClientConnect(const ChannelPtr &channel, TcpClient *cli)
{
  // client_num++;
  // LOG_INFO << "connect fd: " << channel->getFd();
  cli->flush(channel);
}
void onClientPartialRead(const ChannelPtr &channel, const char *buf, int len, TcpClient *cli, TcpServer *serv)
{
  // LOG_INFO << "read fd: " << channel->getFd()
  //          << ", buf: " << buf
  //          << ", len: " << len;
  cliRecived += len;

  TunnelChannel *conn = static_cast<TunnelChannel *>(channel.get());
  if (ChannelPtr tunnel = conn->tunnel.lock())
  {
    // cli->enableRead(channel, true);
    // serv->send(serv_conn, buf, len); //send to tunnel
    if (serv->write(tunnel, buf, len) < 0) 
    { 
      cli->enableRead(channel, false); //make sure stop read before send 
      serv->flush(tunnel);
    }
  }
}
void onClientRead(const ChannelPtr &channel, TcpClient *cli, TcpServer *serv)
{
  TunnelChannel *conn = static_cast<TunnelChannel *>(channel.get());

  if (ChannelPtr tunnel = conn->tunnel.lock())
  { 
    cli->enableRead(channel, false);
    serv->flush(tunnel); //send to tunnel
  }
}
void onClientPartialWrite(const ChannelPtr &channel, const char *msg, int len, TcpClient *endpoint)
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
void onClientWrite(const ChannelPtr &channel, TcpServer *serv)
{
  TunnelChannel *conn = static_cast<TunnelChannel *>(channel.get());

  if (ChannelPtr tunnel = conn->tunnel.lock())
  { 
    LOG_DEBUG << "enableRead: " << tunnel->getFd();
    serv->enableRead(tunnel); //when tunnel is writable, then begin to read again
  }
}

void print()
{

  LOG_INFO_R << "cliRecived: " << (cliRecived >> 10) << ","
             << "cliSent: " << (cliSent >> 10) << ","
             << "servRecived: " << (servRecived >> 10) << ","
             << "servSent: " << (servSent >> 10) << " (kb)";
}

TunnelChannel *createTunnelChannel(int sockfd)
{
  return new TunnelChannel(sockfd);
}
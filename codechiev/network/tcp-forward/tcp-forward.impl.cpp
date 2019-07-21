#include "tcp-forward.impl.hpp"
#include <network/socket.h>

const char *servPort = "1080";
const char *port = "12345";
const char *host = "127.0.0.1";

PipeChannel::PipeChannel(int sockfd) : Channel(sockfd)
{
  session = UUID::GenUUID();
}

PipeChannel *createPipeChannel(int sockfd)
{
  return new PipeChannel(sockfd);
}

void onConnect(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
{ 
  PipeChannel *serv_conn = static_cast<PipeChannel *>(channel.get());
  int conn_sock = Connect(port, host);
  PipeChannel *cli_conn = createPipeChannel(conn_sock);
  serv_conn->pipe = cli_conn->ptr;
  cli_conn->pipe = serv_conn->ptr;
  cli_conn->ptr = ChannelPtr(cli_conn);
  cli->connect(cli_conn);

  LOG_INFO << "connect fd: " << channel->getFd();
}

void onClose(const ChannelPtr &channel)
{
  LOG_INFO << "close fd: " << channel->getFd();
}

void onCliConnect(const ChannelPtr &channel, TcpClient *cli)
{
 
}

void onCliClose(const ChannelPtr &channel)
{ 
}


// bool onPartialRead(const ChannelPtr &channel, const char *buf, int len, TcpServer *serv, TcpClient *cli)
// {
//   // LOG_INFO << "read fd: " << channel->getFd()
//   //       // << ", buf: " << buf
//   //       << ", len: " << len;
//   servRecived += len;

//   TunnelChannel *conn = static_cast<TunnelChannel *>(channel.get());

//   if (ChannelPtr tunnel = conn->tunnel.lock())
//   {
//     if (cli->write(tunnel, buf, len) < 0) 
//     {
//       serv->enableRead(channel, false); //make sure stop read before send 
//       cli->flushData(tunnel);
//       return true;
//     }
//   }
//   return false;
// }
// void onRead(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
// {
//   TunnelChannel *conn = static_cast<TunnelChannel *>(channel.get());

//   if (ChannelPtr tunnel = conn->tunnel.lock())
//   {
//     serv->enableRead(channel, false);
//     cli->flushData(tunnel); //send to tunnel
//   }
// }
// void onPartialWrite(const ChannelPtr &channel, const char *msg, int len, TcpServer *serv)
// {
//   servSent += len;
// }
// void onWrite(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
// {
//   TunnelChannel *conn = static_cast<TunnelChannel *>(channel.get());

//   if (ChannelPtr tunnel = conn->tunnel.lock())
//   {
//     cli->enableRead(tunnel); 
//   }
// }
// void onClose(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
// {
//   TunnelChannel *serv_conn = static_cast<TunnelChannel *>(channel.get());
//   if (ChannelPtr cli_conn = serv_conn->tunnel.lock())
//   {
//     cli->shutdown(cli_conn);
//   }
// }

// void onClientConnect(const ChannelPtr &channel, TcpClient *cli)
// {
//   // client_num++;
//   // LOG_INFO << "connect fd: " << channel->getFd();
//   // printSockopt(channel->getFd());
//   cli->flushData(channel);
// }
// bool onClientPartialRead(const ChannelPtr &channel, const char *buf, int len, TcpClient *cli, TcpServer *serv)
// {
//   // LOG_INFO << "read fd: " << channel->getFd()
//   //          << ", buf: " << buf
//   //          << ", len: " << len;
//   cliRecived += len;

//   TunnelChannel *conn = static_cast<TunnelChannel *>(channel.get());
//   if (ChannelPtr tunnel = conn->tunnel.lock())
//   {
//     // cli->enableRead(channel, true);
//     // serv->send(serv_conn, buf, len); //send to tunnel
//     if (serv->write(tunnel, buf, len) < 0) 
//     {
//       cli->enableRead(channel, false); //make sure stop read before send 
//       serv->flushData(tunnel);
//       return true;
//     }
//   }
//   return false;
// }
// void onClientRead(const ChannelPtr &channel, TcpClient *cli, TcpServer *serv)
// {
//   TunnelChannel *conn = static_cast<TunnelChannel *>(channel.get());

//   if (ChannelPtr tunnel = conn->tunnel.lock())
//   {
//     cli->enableRead(channel, false);
//     serv->flushData(tunnel);
//   }
// }
// void onClientPartialWrite(const ChannelPtr &channel, const char *msg, int len, TcpClient *endpoint)
// {
//   cliSent += len;
// }
// void onClientWrite(const ChannelPtr &channel, TcpClient *cli, TcpServer *serv)
// {
//   TunnelChannel *conn = static_cast<TunnelChannel *>(channel.get());

//   if (ChannelPtr tunnel = conn->tunnel.lock())
//   { 
//     // LOG_DEBUG << "enableRead: " << tunnel->getFd();
//     serv->enableRead(tunnel); //when tunnel is writable, then begin to read again
//   }
// }
// void onClientClose(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
// {
//   TunnelChannel *cli_conn = static_cast<TunnelChannel *>(channel.get());

//   if (ChannelPtr serv_conn = cli_conn->tunnel.lock())
//   {
//     serv->shutdown(serv_conn);
//   }
// }
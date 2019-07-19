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
  cli_conn->ptr = ChannelPtr(cli_conn);
  cli_conn->pipe = serv_conn->ptr;
  serv_conn->pipe = cli_conn->ptr;
  cli->connect(cli_conn);

  LOG_INFO << "connect fd: " << channel->getFd();
}

bool onRead(const ChannelPtr &channel, const char *buf, int len, TcpServer *serv, TcpClient *cli)
{
  LOG_DEBUG << "read fd: " << channel->getFd()
        << ", buf: " << buf
        << ", len: " << len; 

  PipeChannel *conn = static_cast<PipeChannel *>(channel.get());

  if (ChannelPtr pipe = conn->pipe.lock())
  {
    if (cli->write(pipe, buf, len) < 0) 
    {
      serv->disableReading(channel); //make sure stop read before send
      cli->flush(pipe);
      return true;
    }
  }
  return false;
}

void onEndReading(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
{
  PipeChannel *conn = static_cast<PipeChannel *>(channel.get());

  if (ChannelPtr pipe = conn->pipe.lock())
  {
    serv->disableReading(channel);
    cli->flush(pipe);
  }
}

void onEndWriting(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
{
  PipeChannel *conn = static_cast<PipeChannel *>(channel.get());

  if (ChannelPtr pipe = conn->pipe.lock())
  {
    cli->disableReading(pipe, false);
  }
}

void onClose(const ChannelPtr &channel)
{
  PipeChannel *conn = static_cast<PipeChannel *>(channel.get());
  if (ChannelPtr cliConn = conn->pipe.lock())
  {
    cliConn->shutdown();
  }
  LOG_INFO << "close fd: " << channel->getFd();
}

void onCliConnect(const ChannelPtr &channel, TcpClient *cli)
{
  cli->flush(channel);

  LOG_INFO << "connect fd: " << channel->getFd();
}

bool onCliRead(const ChannelPtr &channel, const char *buf, int len, TcpServer *serv, TcpClient *cli)
{
  LOG_DEBUG << "read fd: " << channel->getFd()
        << ", buf: " << buf
        << ", len: " << len; 

  PipeChannel *conn = static_cast<PipeChannel *>(channel.get());

  if (ChannelPtr pipe = conn->pipe.lock())
  {
    if (serv->write(pipe, buf, len) < 0) 
    {
      cli->disableReading(channel);
      serv->flush(pipe);
      return true;
    }
  }
  return false;
}

void onCliEndReading(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
{
  PipeChannel *conn = static_cast<PipeChannel *>(channel.get());

  if (ChannelPtr pipe = conn->pipe.lock())
  {
    cli->disableReading(channel);
    serv->flush(pipe);
  }
}

void onCliEndWriting(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
{
  PipeChannel *conn = static_cast<PipeChannel *>(channel.get());

  if (ChannelPtr pipe = conn->pipe.lock())
  {
    serv->disableReading(pipe, false);
  }
}

void onCliClose(const ChannelPtr &channel)
{
  PipeChannel *conn = static_cast<PipeChannel *>(channel.get());
  if (ChannelPtr servConn = conn->pipe.lock())
  {
    servConn->shutdown();
  }
  LOG_INFO << "close fd: " << channel->getFd();
}




// void onPartialWrite(const ChannelPtr &channel, const char *msg, int len, TcpServer *serv)
// {
//   servSent += len;
// }



// void onClientConnect(const ChannelPtr &channel, TcpClient *cli)
// {
//   // client_num++;
//   // LOG_INFO << "connect fd: " << channel->getFd();
//   // printSockopt(channel->getFd());

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
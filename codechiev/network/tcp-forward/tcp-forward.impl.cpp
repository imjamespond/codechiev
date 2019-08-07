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
  PipeChannel *cli_conn = static_cast<PipeChannel *>(cli->connect(port, host));
  cli_conn->pipe = serv_conn->ptr;
  serv_conn->pipe = cli_conn->ptr; 

  LOG_INFO << channel->getFd();
}

bool onRead(const ChannelPtr &channel, const char *buf, int len, TcpServer *serv, TcpClient *cli)
{
  PipeChannel *conn = static_cast<PipeChannel *>(channel.get());

  if (ChannelPtr pipe = conn->pipe.lock())
  { 
    if (cli->write(pipe, buf, len) < 0) 
    {
      serv->disableReading(channel);
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

  LOG_INFO << channel->getFd();
}

void onClose(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
{
  PipeChannel *conn = static_cast<PipeChannel *>(channel.get());
  if (ChannelPtr cliConn = conn->pipe.lock())
  { 
    cli->shutdown(cliConn); 
  }
  LOG_INFO << channel->getFd();
}

void onCliConnect(const ChannelPtr &channel, TcpClient *cli)
{
  cli->flush(channel);

  LOG_INFO << channel->getFd();
}

bool onCliRead(const ChannelPtr &channel, const char *buf, int len, TcpServer *serv, TcpClient *cli)
{
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

  LOG_INFO << channel->getFd();
}

void onCliClose(const ChannelPtr &channel, TcpServer *serv, TcpClient *cli)
{
  PipeChannel *conn = static_cast<PipeChannel *>(channel.get());
  if (ChannelPtr servConn = conn->pipe.lock())
  {
    serv->shutdown(servConn); 
  }
  LOG_INFO << channel->getFd();
}
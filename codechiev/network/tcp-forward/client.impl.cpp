#include "tcp-forward.impl.hpp"
#include <network/socket.h>


void onCliConnect(const ChannelPtr &channel, TcpClient *cli)
{
  cli->flush(channel);

  LOG_INFO << channel->getFd();
}

void onCliRead(const ChannelPtr &channel, const char *buf, int len, TcpServer *serv, TcpClient *cli)
{
  PipeChannel *conn = static_cast<PipeChannel *>(channel.get());

  if (ChannelPtr pipe = conn->pipe.lock())
  {
    if (serv->write(pipe, buf, len) < 0) 
    {
      cli->disableReading(channel);
      serv->flush(pipe); 
    }
  } 
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

  // LOG_INFO << channel->getFd();
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
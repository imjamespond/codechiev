#include "TcpEndpoint.hpp"
#include "socket.h"

#include <errno.h>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

#define __BUFF_SPAN__ 1024

TcpEndpoint::TcpEndpoint() : onConnect(0),
                             onClose(0),
                             onRead(0), onWrite(0),
                             onReadEnd(0), onWritten(0),
                             createChannel(0)
{
}

void TcpEndpoint::handle(const ChannelPtr &channel)
{
  if (channel->readable())
  {
    char buffer[__BUFF_SPAN__];
    size_t buf_len = sizeof buffer;

    for (;;)
    {
      ::memset(buffer, 0, buf_len);
      ssize_t len = ::read(channel->getFd(), buffer, buf_len);

      LOG_DEBUG
          << "read fd: " << channel->getFd()
          << ", len: " << len
          << ", errno: " << errno;

      if (len > 0)
      {
        if (onRead && onRead(channel, buffer, len))
        {
          break;
        }
      }
      else if (len && -1 == len)
      {
        if (errno == EAGAIN)
        {
          if (onReadEnd)
          {
            onReadEnd(channel);
          }
          break;
        }
      }
      else
      {
        channel->setClosed();
        break;
      }
    }

    flush(channel); //reading end and check if there is something to write in the channel!
  }

  else if (channel->writable())
  {
    handleWrite(channel);
  }
  if (channel->closed)
  {
    if (onClose)
    {
      onClose(channel);
    }
    close(channel);
  }
}

void TcpEndpoint::handleWrite(const ChannelPtr &channel)
{
  bool done(false);

  for (;;)
  {
    ssize_t len;
    {
      len = ::write(channel->getFd(), channel->buffer.buf(), channel->buffer.readable_bytes());

      LOG_DEBUG
          << "write fd: " << channel->getFd()
          << ", len: " << len
          << ", errno: " << errno;

      if (len > 0)
      {
        if (onWrite)
        {
          onWrite(channel, channel->buffer.buf(), len);
        }

        channel->buffer.read(len);
        channel->buffer.move();
      }
      else if (-1 == len)
      {
        // TODO check writting buffer of the channel which might not be sent completely
        if (errno == EAGAIN)
        {
          done = true;
          break;
        }
        else if (errno == EPIPE)
        {
          channel->setClosed(); // broken pipe
          break;
        }
      }
      else if (0 == len)
      {
        // 0 will be returned without causing any other effect
        done = true;
        break;
      }
    }
    // SetTcpNoDelay(channel->getFd());
  }

  if (done)
  {
    written(channel);
  }
}

void TcpEndpoint::written(const ChannelPtr &channel)
{
  assert(channel->loop);

  channel->loop
      ->getPoll()
      ->setReadable(channel.get(), channel->readDisabled ? __EVENT_HUP__ : __EVENT_READ__);

  if (onWritten)
  {
    onWritten(channel);
  }
}

int TcpEndpoint::write(const ChannelPtr &channel, const char *buf, int len)
{

  if (channel->buffer.append(buf, len) < 0)
  {
    LOG_ERROR << "append to buffer failed: " << len
              << ", readable_bytes: " << channel->buffer.readable_bytes()
              << ", writable_bytes: " << channel->buffer.writable_bytes();
    return -1;
  }

  int readable = channel->buffer.readable_bytes();
  const int max = Channel::BufferSize - __BUFF_SPAN__;
  if (readable > max)
  {
    return -1;
  }

  return 0;
}

void TcpEndpoint::flush(const ChannelPtr &channel)
{
  assert(channel->loop);

  if (channel->connected && channel->buffer.readable_bytes())
  {
    channel->loop
        ->getPoll()
        ->setWritable(channel.get());

    LOG_DEBUG << "flush fd: " << channel->getFd();
  }
}

void TcpEndpoint::send(const ChannelPtr &channel, const char *msg, int len)
{
  write(channel, msg, len);
  flush(channel);
}

void TcpEndpoint::disableReading(const ChannelPtr &channel, bool disable)
{
  assert(channel->loop);

  channel->disableReading(disable);

  channel->loop
      ->getPoll()
      ->setReadable(channel.get(), disable ? __EVENT_HUP__ : __EVENT_READ__);

  LOG_DEBUG << "disableReading: " << channel->getFd() << ", disable: " << disable;
}

void TcpEndpoint::close(const ChannelPtr &channel)
{
  channel->loop->getPoll()->ctlDel(channel.get());
  channel->shutdown();
  channel->ptr.reset();
}

void TcpEndpoint::shutdown(const ChannelPtr &channel)
{
  channel->setClosed();
}
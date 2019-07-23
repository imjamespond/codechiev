#include "TcpEndpoint.hpp"
#include "socket.h"

#include <errno.h>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

#define BUFF_SPAN 1024

TcpEndpoint::TcpEndpoint(bool edge_mode) : edge_mode(edge_mode),
                                           event_read(EPOLLIN | (edge_mode ? EPOLLET : 0)),
                                           onConnect(0), onClose(0),
                                           onRead(0), onWrite(0),
                                           onEndReading(0), onEndWriting(0),
                                           createChannel(0)
{
}

void TcpEndpoint::handle_event_(const ChannelPtr &channel)
{
  if (channel->readable())
  {
    char buffer[BUFF_SPAN];
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
          if (onEndReading)
          {
            onEndReading(channel);
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
  }

  else if (channel->writable())
  {
    bool writing_done(false);
    {
      MutexGuard lock(&mutex);

      if (channel->closing() && !channel->closed())
      {
        channel->shutdown();
        channel->setClosed();
        return;
      }

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
              writing_done = true;
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
            writing_done = true;
            break;
          }
        }
        // SetTcpNoDelay(channel->getFd());
      }
    }

    if (writing_done)
    {
      writing_done_(channel);
    }
  }
  else
  {
    LOG_DEBUG << "????";
  }
}

void TcpEndpoint::writing_done_(const ChannelPtr &channel)
{
  assert(channel->loop);

  {
    MutexGuard lock(&mutex);

    channel->setWriting(false);

    reinterpret_cast<Loop *>(channel->loop)
        ->getPoll()
        ->setReadable(channel.get(), channel->readingDisabled() ? EVENT_HUP_ : event_read);
  }
      
  if (onEndWriting)
  {
    onEndWriting(channel); // might lock other thread, so release lock above
  }
}

void TcpEndpoint::close_(Loop *loop, const ChannelPtr &channel)
{
  loop->getPoll()->ctlDel(channel.get());

  channel->ptr.reset();
}

int TcpEndpoint::write(const ChannelPtr &channel, const char *buf, int len)
{
  MutexGuard lock(&mutex);

  if (channel->buffer.append(buf, len) < 0)
  {
    LOG_ERROR << "append to buffer failed: " << len
              << ", readable_bytes: " << channel->buffer.readable_bytes()
              << ", writable_bytes: " << channel->buffer.writable_bytes();
    return -1;
  }

  int writable = channel->buffer.writable_bytes(); 
  if (writable < BUFF_SPAN)
  {
    channel->buffer.resize(writable, BUFF_SPAN);
    if (writable < BUFF_SPAN)
    {
      return -1;
    }
  }

  return 0;
}

void TcpEndpoint::flush(const ChannelPtr &channel)
{
  assert(channel->loop);

  MutexGuard lock(&mutex);

  if (channel->buffer.readable_bytes())
  {
    channel->setWriting();

    reinterpret_cast<Loop *>(channel->loop)
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

  {
    MutexGuard lock(&mutex);

    channel->disableReading(disable);

    if (!channel->writing())
    {
      reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
          ->getPoll()
          ->setReadable(channel.get(), disable ? EVENT_HUP_ : event_read);
    }
  }


  LOG_DEBUG << "disableReading: " << channel->getFd() << ", disable: " << disable;
}

void TcpEndpoint::shutdown(const ChannelPtr &channel)
{ 
  MutexGuard lock(&mutex); 

  // if (!channel->closing())
  // {
    channel->setClosing();

    assert(channel->loop);
    reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
        ->getPoll()
        ->setWritable(channel.get());
  // }
}
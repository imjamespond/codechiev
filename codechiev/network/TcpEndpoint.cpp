#include "TcpEndpoint.hpp"
#include "socket.h"

#include <errno.h>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

Channel *__create_channel__(int sockfd)
{
  return new Channel(sockfd);
}

TcpEndpoint::TcpEndpoint(bool _edge) : edge(_edge ? EPOLLET : 0), onConnect(0), onPartialRead(0), onPartialWrite(0), onWrite(0), onClose(0), createChannel(boost::bind(&__create_channel__, _1))
{
  // LOG_DEBUG << "TcpEndpoint";
}

void TcpEndpoint::_handle_event(const ChannelPtr &channel)
{
  if (channel->event_read())
  {
    // LOG_DEBUG << "debug level triggered";

    char buffer[Channel::BufferSize];
    size_t buf_len = sizeof buffer;
    for (;;)
    {
      if (_read_check(channel))
      {
        break;
      }

      ::memset(buffer, 0, buf_len);
      ssize_t len = ::read(channel->getFd(), buffer, buf_len);

      LOG_DEBUG << "read fd: " << channel->getFd() << ", len: " << len << ", errno: " << errno;

      if (len > 0)
      {
        // LOG_DEBUG << "buffer: " << buffer;
        if (onPartialRead)
        {
          onPartialRead(channel, buffer, len);
        }

      }
      else if (len && -1 == len)
      {
        if (errno == EAGAIN)
        {
          // LOG_DEBUG << "EAGAIN";
          if (onRead) 
            onRead(channel);
          break;
        }
      }
      else
      {
        // LOG_DEBUG << "close fd: " << channel->getFd();
        if (onClose)
          onClose(channel);

        channel->set_closed(); // channel will be delete after return
        break;
      }
    }
  }
  //write
  else if (channel->event_write())
  {
    for (;;)
    {
      ssize_t len;
      {
        MutexGuard lock(&mutex);
        len = ::write(channel->getFd(), channel->buffer.buf(), channel->buffer.readable_bytes());

        if (len > 0)
        {
          if (onPartialWrite)
            onPartialWrite(channel, channel->buffer.buf(), len);

          channel->buffer.read(len);
          channel->buffer.move();
        }
      }
      // SetTcpNoDelay(channel->getFd());

      // LOG_DEBUG << "write fd: " << channel->getFd()
      //   << ", readable: " << channel->buffer.readable_bytes()
      //   << ", len: " << len << ", errno: " << errno;

      if (len && -1 == len)
      {
        // TODO check writting buffer of the channel which might not be sent completely
        if (errno == EAGAIN)
        {
          // LOG_DEBUG << "write EAGAIN" ;

          _writing_done(channel);
          break;
        }
        else if (errno == EPIPE)
        {
          channel->set_closed(); // broken pipe
          break;
        }
      }
      else
      {
        // 0 will be returned without causing any other effect
        _writing_done(channel);
        break;
      }
    }
  }
  //close
  else if (channel->is_closed())
  {
    LOG_DEBUG << "EPOLLHUP: " << channel->getFd();

    if (onClose)
      onClose(channel);
  }
}

void TcpEndpoint::_writing_done(const ChannelPtr &channel)
{

  assert(channel->loop);
  reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
      ->getPoll()
      ->setReadable(channel.get(), EPOLLIN | edge);

  {
    MutexGuard lock(&mutex);
    channel->set_write_action(false);
  }

  if (onWrite)
  {
    onWrite(channel);
  }

  if (channel->is_closing())
  {
    //  LOG_DEBUG << "shut down channel: " << channel->getFd();

    channel->shutdown();
  }
}

void TcpEndpoint::_close(Eventloop<Epoll> *loop, const ChannelPtr &channel)
{
  //unable to epoll_ctl: mod: No such file or directory
  loop->getPoll()->ctlDel(channel.get());
  //unable to epoll_ctl: mod: Bad file descriptor
  // channel->close(); // should close in destruct, avoid mod old pointer along with new fd

  // delete channel;
  Channel::ChannelPtr _channel;
  channel->ptr.swap(_channel);
}

int TcpEndpoint::write(const ChannelPtr &channel, const char *buf, int len)
{
  MutexGuard lock(&mutex);
  // do not set writable again
  if (!channel->is_closing())
  {
    if (channel->buffer.append(buf, len) < 0)
    {
      // LOG_ERROR << "append to buffer failed: " << len
      //           << ", readable_bytes: " << channel->buffer.readable_bytes()
      //           << ", writable_bytes: " << channel->buffer.writable_bytes();
      return -1;
    }

    if (channel->buffer.writable_bytes() < Channel::BufferSize)
    {
      return -1; // reserve for next read
    }
  }
  return 0;
}

void TcpEndpoint::flush(const ChannelPtr &channel)
{
  if (channel->loop)
  {
    if (reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
            ->getPoll()
            ->setWritable(channel.get()) < 0)
    {
      // channel->set_closing();
    }
  }
}

void TcpEndpoint::flushData(const ChannelPtr &channel)
{
  MutexGuard lock(&mutex);

  if (channel->buffer.readable_bytes())
  {
    channel->set_write_action(); 
    flush(channel);
  }
}

void TcpEndpoint::send(const ChannelPtr &channel, const char *msg, int len)
{
  write(channel, msg, len);
  flush(channel);
}

void TcpEndpoint::shutdown(const ChannelPtr &channel)
{
  // LOG_DEBUG << "shut down: " << channel->getFd();
  MutexGuard lock(&mutex);
  // do not set writable again
  if (!channel->is_closing())
  {
    channel->set_closing();

    assert(channel->loop);
    reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
        ->getPoll()
        ->setWritable(channel.get());
  }
}

void TcpEndpoint::enableRead(const ChannelPtr &channel, bool enable)
{
  MutexGuard lock(&mutex);

  channel->enable_read(enable);
  
  if (enable && !channel->write_action())
  {
    assert(channel->loop);
    reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
        ->getPoll()
        ->setReadable(channel.get(), EPOLLIN | edge); 
  }
}

bool TcpEndpoint::_read_check(const ChannelPtr &channel)
{
  MutexGuard lock(&mutex);

  if (channel->write_action())
  {
    return true;
  }
  if (channel->read_disabled())
  {
    LOG_DEBUG << "read_disabled: " << channel->getFd();

    assert(channel->loop);
    reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
        ->getPoll()
        ->setReadable(channel.get(), (EPOLLHUP | EPOLLRDHUP | EPOLLERR)); 
    return true;
  }
  return false;
}
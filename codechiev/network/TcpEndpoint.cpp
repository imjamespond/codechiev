#include "TcpEndpoint.hpp"
#include "socket.h"

#include <errno.h>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

Channel* __create_channel__(int sockfd)
{
  return new Channel(sockfd);
}

TcpEndpoint::TcpEndpoint() : onConnect(0),  onRead(0),  onWrite(0),  onClose(0), createChannel(boost::bind(&__create_channel__, _1))
{
  // LOG_DEBUG << "TcpEndpoint";
}

void TcpEndpoint::_handle_event(const ChannelPtr &channel)
{
  if (channel->isReadable())
  {
    char buffer[256];
    size_t buf_len = sizeof buffer;
    for (;;)
    {
      ::memset(buffer, 0, buf_len);
      ssize_t len = ::read(channel->getFd(), buffer, buf_len);

      // LOG_DEBUG << "fd: " << channel->getFd() << ", len: " << len << ", errno: " << errno;

      if (len > 0)
      {
        // LOG_DEBUG << "buffer: " << buffer;
        if (onRead)
          onRead(channel, buffer, len);
      }
      else if (len && -1 == len)
      {
        if (errno == EAGAIN)
        {
          // LOG_DEBUG << "EAGAIN";
          break;
        }
      }
      else
      {
        // LOG_DEBUG << "close fd: " << channel->getFd();
        if (onClose)
          onClose(channel);

        channel->setClosed(); // channel will be delete after return
        break;
      }
    }
  }
  //write
  else if (channel->isWritable())
  {
    for (;;)
    {
      ssize_t len;
      {
        MutexGuard lock(&mutex);
        len = ::write(channel->getFd(), channel->buffer.buf(), channel->buffer.readable_bytes());
      }
      // SetTcpNoDelay(channel->getFd());

      // LOG_DEBUG << "write fd: " << channel->getFd()
      //   << ", readable: " << channel->buffer.readable_bytes()
      //   << ", len: " << len << ", errno: " << errno;

      if (len > 0)
      {
        // TODO channel buffer need to be protected
        MutexGuard lock(&mutex);

        if (onWrite)
          onWrite(channel, channel->buffer.buf(), len);

        channel->buffer.read(len);
        channel->buffer.move(); 
      }
      else if (len && -1 == len)
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
          channel->setClosed();// broken pipe
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
  else if (channel->isClosed())
  {
    LOG_DEBUG << "EPOLLHUP detected, fd: " << channel->getFd();

    if (onClose)
      onClose(channel);
  }
}

void TcpEndpoint::_writing_done(const ChannelPtr &channel)
{
  assert(channel->loop);
  reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
      ->getPoll()
      ->setReadable(channel.get());

  if (channel->isClosing())
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


void TcpEndpoint::send(const ChannelPtr &channel, const char *msg, int len, bool flush)
{
  // do not set writable again
  if (!channel->isClosing())
  {
    {
      MutexGuard lock(&mutex);
      if (channel->buffer.append(msg, len) < 0)
        LOG_ERROR << "append to buffer failed: " << len
                  << ", readable_bytes: " << channel->buffer.readable_bytes()
                  << ", writable_bytes: " << channel->buffer.writable_bytes();
    }
    
    if (channel->loop && flush)
    {
      if (reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
          ->getPoll()
          ->setWritable(channel.get()) < 0)
        channel->setClosed();
    }

  }
}

void TcpEndpoint::shutdown(const ChannelPtr & channel)
{
    // LOG_DEBUG << "shut down: " << channel->getFd();

   // do not set writable again
   if (!channel->isClosing())
   {
     channel->setClosing();

     assert(channel->loop);
     reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
         ->getPoll()
         ->setWritable(channel.get());
  } 
}

#include "TcpEndpoint.hpp"
#include "Eventloop.hpp"
#include "Epoll.hpp"

#include <errno.h>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpEndpoint::TcpEndpoint() : onConnect(0),  onRead(0),  onWrite(0),  onClose(0)
{
  LOG_DEBUG << "TcpEndpoint";
}

void TcpEndpoint::_handleEvent(Channel *channel)
{
  if (channel->isReadable())
  {
    char buf[32];
    size_t buf_len = sizeof buf;
    for (;;)
    {
      ::memset(buf, 0, buf_len);
      ssize_t len = ::read(channel->getFd(), buf, buf_len);

      // LOG_DEBUG << "fd: " << channel->getFd() << ", len: " << len << ", errno: " << errno;

      if (len > 0)
      {
        // LOG_DEBUG << "buf: " << buf;
        if (onRead)
          onRead(channel, buf, len);
      }
      else if (len)
      {
        if (-1 == len && errno == EAGAIN)
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
      ssize_t len = ::write(channel->getFd(), channel->buf.str(), channel->buf.readable_bytes());

      // LOG_DEBUG << "write fd: " << channel->getFd() << ", len: " << len << ", errno: " << errno;

      if (len > 0)
      {
        // TODO channel buf need to be protected
        MutexGuard lock(&mutex);

        if (onWrite)
          onWrite(channel, channel->buf.str(), len);

        channel->buf.read(len);
        channel->buf.move(); 
      }
      else if (len)
      {
        // TODO check writting buf of the channel which might not be sent completely
        if (-1 == len && errno == EAGAIN)
        {
          // LOG_DEBUG << "write EAGAIN" ;

          _writtingDone(channel);
          break;
        }
      }
      else 
      {
        // 0 will be returned without causing any other effect
        _writtingDone(channel);
        break;
      }
    }
  }
  //close
  else if (channel->isClosed())
  {
    LOG_DEBUG << "shut down writing half of connection fd : " << channel->getFd();

    if (onClose)
      onClose(channel);
  }

}

void TcpEndpoint::send(Channel *channel, const char *msg, int len)
{
  // do not set writable again
  if (!channel->isClosable())
  {
    {
      MutexGuard lock(&mutex);
      channel->buf.append(msg, len);
    }
    
    assert(channel->loop);
    reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
        ->getPoll()
        ->setWritable(channel);
  }
}

void TcpEndpoint::shutdown(Channel *channel)
{
   // do not set writable again
  if (!channel->isClosable())
  {
    channel->setClosable();

    assert(channel->loop);
    reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
        ->getPoll()
        ->setWritable(channel);
  } 
}

void TcpEndpoint::_writtingDone(Channel *channel)
{
  assert(channel->loop);
  reinterpret_cast<Eventloop<Epoll> *>(channel->loop)
      ->getPoll()
      ->setReadable(channel);

  if (channel->isClosable())
  {
    channel->shutdown();
  }
}

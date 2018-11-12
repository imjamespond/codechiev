#include "TcpEndpoint.hpp"
#include "Eventloop.hpp"

#include <errno.h>
#include <base/Logger.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

void TcpEndpoint::handler(Channel *channel)
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
        channel->setClosable();// channel will be delete after return 
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
        if (onWrite)
          onWrite(channel, channel->buf.str(), len);

        channel->buf.read(len);
      }
      else if (-1 == len || 0 == len)
      {
        // TODO check writing buf of the channel which might not be sent completely
        if (errno == EAGAIN)
        {
          // LOG_DEBUG << "write EAGAIN";
          epoll.setReadable(channel);
          break;
        }
      }
    }

  }
  //close
  else if (channel->isClosable())
  {
    LOG_DEBUG << "shut down writing half of connection fd : " << channel->getFd();

    if (onClose)
      onClose(channel);
  }
}
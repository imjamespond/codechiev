#include "TcpEndpoint.hpp" 

#include <errno.h>
#include <base/Logger.hpp> 

using namespace codechiev::base;
using namespace codechiev::net;


void TcpEndpoint::handleWrite(const ChannelPtr &channel)
{
  bool done(false);

  for (;;)
  {
    ssize_t len;
    {
      len = ::write(channel->getFd(), channel->buffer.buf(), channel->buffer.readable_bytes());

      // LOG_DEBUG
      //     << "write fd: " << channel->getFd()
      //     << ", len: " << len
      //     << ", errno: " << errno;

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
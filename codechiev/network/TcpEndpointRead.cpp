#include "TcpEndpoint.hpp" 

#include <errno.h>
#include <base/Logger.hpp> 

using namespace codechiev::base;
using namespace codechiev::net;


void TcpEndpoint::handleRead(const ChannelPtr &channel)
{
  char buffer[__BUFF_SPAN__];
  size_t buf_len = sizeof buffer;

  for (;;)
  {

    ::memset(buffer, 0, buf_len);
    ssize_t len = ::read(channel->getFd(), buffer, buf_len);

    // LOG_DEBUG
    //     << "read fd: " << channel->getFd()
    //     << ", len: " << len
    //     << ", errno: " << errno;

    if (len > 0)
    {
      if (onRead)// && onRead(channel, buffer, len))
      {
        onRead(channel, buffer, len);

        if (channel->readDisabled) 
        {
          break;
        } 
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
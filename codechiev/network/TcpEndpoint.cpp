#include "TcpEndpoint.hpp"
#include "socket.h"

#include <errno.h>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpEndpoint::TcpEndpoint(bool edge_mode) : edge_mode(edge_mode),
                                           onConnect(0), onClose(0),
                                           onRead(0), onWrite(0),
                                           onEndReading(0), onEndWriting(0)
{
}

void TcpEndpoint::handle_event_(const ChannelPtr &channel)
{
  if (channel->readable())
  {
    char buffer[1024];
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
            onEndReading(channel);
          break;
        }
      }
      else
      {
        if (onClose)
          onClose(channel);
        channel->setClosed(); 
        break;
      }
    }
  }
  else if (channel->writable())
  {

  }
  else 
  {
    LOG_DEBUG<<"????";
  }
  
}

void TcpEndpoint::writing_done_(const ChannelPtr &channel)
{
}

void TcpEndpoint::close_(Eventloop<Epoll> *loop, const ChannelPtr &channel)
{
  loop->getPoll()->ctlDel(channel.get());

  channel->ptr.reset();
}

int TcpEndpoint::write(const ChannelPtr &channel, const char *buf, int len)
{
  
  return 0;
}

void TcpEndpoint::flush(const ChannelPtr &channel)
{

}

void TcpEndpoint::flushData(const ChannelPtr &channel)
{
  MutexGuard lock(&mutex);

}

void TcpEndpoint::send(const ChannelPtr &channel, const char *msg, int len)
{
  write(channel, msg, len);
  flush(channel);
}

void TcpEndpoint::shutdown(const ChannelPtr &channel)
{

}

void TcpEndpoint::enableRead(const ChannelPtr &channel, bool enable)
{

}

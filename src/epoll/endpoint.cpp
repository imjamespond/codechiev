#include <errno.h>
#include "endpoint.hpp"

using namespace learn_cpp;

Endpoint::Endpoint(Endpoint::Loop *_loop) : loop(_loop)
{
}

void Endpoint::handleRead(const ChannelPtr &chan)
{
  char buffer[1 << 10];
  std::size_t buf_len = sizeof buffer;

  for (;;)
  {
    ::memset(buffer, 0, buf_len);
    ssize_t len = ::read(chan->GetFd(), buffer, buf_len);

    if (len > 0 && onRead)
    {
      onRead(chan, buffer, len);
    }
    else if (-1 == len)
    {
      if (errno == EAGAIN)
      {
        if (onReadEnd)
        {
          onReadEnd(chan);
        }
        break;
      }
    }
    else
    {
      break;
    }
  }
}

void Endpoint::handleWrite(const ChannelPtr &chan)
{

  MutexGuard lock(&mutex);
  uint written(0);
  for (;;)
  {
    if (queue.size() == 0)
    {
      loop->GetPoll()->CtlMod(chan.get(), __EVENT_READ__);
      break;
    }
    std::string str = queue.front();
    ssize_t len;
    len = ::write(chan->GetFd(), str.c_str() + written, str.length() - written);

    if (len > 0)
    {
      written += len;
      if (written + 1 >= str.length())
      {
        queue.pop();
      }
    }
    else if (-1 == len)
    {
      if (errno == EAGAIN)
      {
        loop->GetPoll()->CtlMod(chan.get(), __EVENT_READ__);
        break;
      }
      else if (errno == EPIPE)
      {
        // chan1->setClosed(); // broken pipe
        break;
      }
    }
    else if (0 == len)
    {
      break;
    }
  }
}

void Endpoint::Write(const ChannelPtr &chan, const std::string &str)
{
  {
    MutexGuard lock(&mutex);
    queue.push(str);
  }
  loop->GetPoll()->CtlMod(chan.get(), __EVENT_WRITE__);
}
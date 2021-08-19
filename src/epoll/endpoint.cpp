#include <errno.h>
#include "endpoint.hpp"

using namespace learn_cpp;

Endpoint::Endpoint(Endpoint::Loop *_loop) : loop(_loop)
{
}

void Endpoint::handleRead(const ChannelPtr &chan)
{
  char buffer[1 << 2];
  std::size_t buf_len = sizeof buffer;

  for (;;)
  {
    ::memset(buffer, 0, buf_len);
    ssize_t len = ::read(chan->GetFd(), buffer, buf_len);

    if (len > 0 && onRead)
    {
      onRead(chan, buffer, len);
    }
    else if (-1 == len && errno == EAGAIN)
    {
      if (onReadEnd)
      {
        onReadEnd(chan);
      }
      break;
    }
    else
    {
      loop->GetPoll()->CtlDel(chan.get());
      chan->Reset();
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
    std::deque<std::string> &queue = chan->queue;
    if (queue.size() == 0)
    {
      break;
    }
    std::string str = queue.front();
    ssize_t len(0);
    int size = str.length() - written;
    len = ::write(chan->GetFd(), str.c_str() + written, size); // limit size > 4 ? 4 : size

    if (len > 0)
    {
      if ((written += len) >= str.length())
      {
        queue.pop_front();
      }
      if (onWrite)
      {
        onWrite(chan, NULL, len);
      }
    }
    else if (-1 == len)
    {
      if (errno == EAGAIN)
      {
        if (onWritten)
        {
          onWritten(chan);
        }
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

  loop->GetPoll()->CtlMod(chan.get(), __EVENT_READ__);
}

void Endpoint::Write(const ChannelPtr &chan, const std::string &str)
{
  std::deque<std::string> &queue = chan->queue;
  {
    MutexGuard lock(&mutex);
    queue.push_back(str);
  }
}

void Endpoint::SetWriteEvent(const ChannelPtr &chan)
{
  loop->GetPoll()->CtlMod(chan.get(), __EVENT_WRITE__);
}
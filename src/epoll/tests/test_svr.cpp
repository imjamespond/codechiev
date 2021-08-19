#include <utils/log.hpp>
#include <epoll/server.hpp>

using namespace learn_cpp;
using namespace boost::placeholders;

namespace lc = learn_cpp;

void connect(const ChannelPtr &chan)
{
  Log() << "connect " << chan->GetFd();
}
void read_func(const ChannelPtr &chan, const char *msg, int size, Server *svr)
{
  chan->buffer += std::string(msg, size);
  Log() << "read " << size << ", fd " << chan->GetFd();
}
void readend_func(const ChannelPtr &chan, Server *svr)
{
  svr->Write(chan, chan->buffer.c_str());
  svr->SetWriteEvent(chan);
  chan->buffer.clear();
  Log() << "read end " << chan->GetFd();
}
void write_func(const ChannelPtr &chan, const char *msg, int size)
{
  Log() << "write " << size << ", fd " << chan->GetFd();
}

void send();

int main(int num, const char **args)
{
  Endpoint::Loop loop;
  Server svr(&loop);
  svr.onConnect = boost::bind(&connect, _1);
  svr.onRead = boost::bind(&read_func, _1, _2, _3, &svr);
  svr.onReadEnd = boost::bind(&readend_func, _1, &svr);
  svr.onWrite = boost::bind(&write_func, _1, _2, _3);

  loop.Loop();
  loop.Join();
  return 1;
}

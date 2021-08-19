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
  std::string str(msg, size);
  svr->Write(chan, str);
  Log() << "read " << size << ", fd " << chan->GetFd();
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
  svr.onRead = boost::bind(&read_func, _1, _2, _3,  &svr);
  svr.onWrite = boost::bind(&write_func, _1, _2, _3);
 
  loop.Loop(); 
  loop.Join();
  return 1;
}

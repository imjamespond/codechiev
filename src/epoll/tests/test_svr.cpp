#include <utils/log.hpp>
#include <epoll/server.hpp>

using namespace learn_cpp;
using namespace boost::placeholders;
namespace lc = learn_cpp;

void connect(const ChannelPtr &chan)
{
  Log() << "connect " << chan->GetFd();
}
void read_func(const ChannelPtr &chan, const char *msg, int size)
{
  std::string str(msg, size);
  Log() << "read " << str << ", fd " << chan->GetFd() ;
}

int main(int num, const char **args)
{

  Endpoint::Loop loop;
  Server svr(&loop);
  svr.onConnect = boost::bind(&connect, _1);
  svr.onRead = boost::bind(&read_func, _1, _2, _3);

  loop.Loop();
  loop.Join();
  return 1;
}
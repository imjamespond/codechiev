#include <utils/log.hpp>
#include <epoll/channel.hpp>

namespace lc = learn_cpp;

int main(int num, const char **args)
{

  lc::Channel *chan = lc::Channel::Create(99);

  {
    lc::ChannelPtr chanPtr = chan->GetPtr();
    Log() << chanPtr.use_count();
  }

  Log() << "reset";
  chan->Reset();

  return 1;
}
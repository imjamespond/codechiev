#include <network/Eventloop.hpp>
#include <base/Logger.hpp>
#include <base/Time.hpp>

namespace ccnet = codechiev::net;
namespace ccbase = codechiev::base;

class Foo 
{
  public:
    void wait() 
    {
      LOG_INFO;
      ccbase::Time::SleepMillis(100);
    }
};

int main(int num, const char **args)
{
  ccnet::Eventloop<Foo> loop;

  loop.loop();

  return 1;
}
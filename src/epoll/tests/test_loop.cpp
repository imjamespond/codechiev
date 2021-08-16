#include <stdio.h>
#include <unistd.h>
#include <epoll/loop.hpp> 
#include <boost/log/trivial.hpp>

namespace lc = learn_cpp; 

const long MicroSec = 1;
const long MilliSec = MicroSec * 1000;
const long Sec = MilliSec * 1000;

class DummyPool 
{
  public:
    void Wait() 
    {
      ::printf("usleep\n");
      ::usleep( Sec * 3 );
      ::printf("end\n");
    }
};

int main(int num, const char **args)
{
  lc::Eventloop<DummyPool> loop;

  loop.loop();

  return 1;
}
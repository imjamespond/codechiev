#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <errno.h> 
// #include <signal.h>
#include <boost/bind.hpp>

#include <network/Timer.hpp>  

#include <base/Time.hpp>
#include <base/Logger.hpp>
#include <base/Thread.hpp>
#include <base/Keyboard.hpp>

using namespace codechiev::net;
using namespace codechiev::base;

Timer timer;

void input();
void print(int val)
{
  LOG_INFO << "print: "<< val;
}

int main( )
{

  // struct sigaction st[] = {SIG_IGN};
  // sigaction(SIGPIPE, st, NULL);


  Eventloop<Epoll> loop; 
  timer.start(&loop);

  input();

  return 1;
}

void input()
{
  char string[256];
  for (;;)
  {
    if (keyboard::fgets(string, 100) != NULL)
    {
      printf("fgets: %s", string);
      if (::strcmp(string, "set\n") == 0)
      {
        timer.schedule(boost::bind(&print,3), 3000l, 0);
        timer.schedule(boost::bind(&print,2), 2000l, 0);
        timer.schedule(boost::bind(&print,1), 1000l, 0);
        LOG_INFO << "setTimer";
      }
      else if (::strcmp(string, "set-3\n") == 0)
      {
        timer.schedule(boost::bind(&print,0), 3000l, 1000l, 3);
        LOG_INFO << "setTimer";
      }
      else if (::strcmp(string, "repeat\n") == 0)
      {
        timer.schedule(boost::bind(&print,0), 3000l, 1000l, -1);
        LOG_INFO << "repeatTimer";
      }
    }
  }

}

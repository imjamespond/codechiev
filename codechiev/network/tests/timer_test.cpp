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
void timesup()
{
  LOG_INFO << "timesup";
}

int main( )
{

  // struct sigaction st[] = {SIG_IGN};
  // sigaction(SIGPIPE, st, NULL);


  Eventloop<Epoll> loop;
  timer.setHandler( boost::bind(&timesup));
  timer.start(loop);

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
        timer.schedule(3000l, 1000l);
        LOG_INFO << "setTimer";
      } 
    }
  }

}

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

int main( )
{

  // struct sigaction st[] = {SIG_IGN};
  // sigaction(SIGPIPE, st, NULL);

  Thread t_input("input", boost::bind(&input));
  t_input.start();

  Eventloop<Epoll> loop;
  timer.start(loop);

  t_input.join();

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

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

void testTasks();
void input();
void print(int val)
{
  LOG_INFO << "print: "<< val;
}

int main( )
{

  // struct sigaction st[] = {SIG_IGN};
  // sigaction(SIGPIPE, st, NULL);

  // testTasks();

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


void testTasks()
{
  Timer::Tasks tasks;
  Time now = Time::NowClock();

  Timer::Task task1(boost::bind(&print,1), now, 1000l, 0);
  tasks.push_back(task1);
  Timer::Task task2(boost::bind(&print,2), now, 1000l, 1);
  tasks.push_back(task2);

  Timer::Tasks::iterator it = tasks.begin();
  Timer::Tasks::iterator lastest = tasks.end();
  while (it != tasks.end())
  {
    
    if (it == tasks.begin())
    {
      Timer::Task _task(boost::bind(&print,0), now, 1000l, 1);
      it = tasks.erase(it);//it points to 2
      it = tasks.insert(it, _task);// it points to 0
    }

    Timer::Task &task = *it;
    task.func();

    ++it;
  }
}
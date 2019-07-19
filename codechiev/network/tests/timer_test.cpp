#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <errno.h>  
#include <boost/bind.hpp>

#include <network/Timer.hpp>  

#include <base/Time.hpp>
#include <base/Logger.hpp>
#include <base/Thread.hpp>
#include <base/Keyboard.hpp>

using namespace codechiev::net;
using namespace codechiev::base;

Timer timer1;
Timer timer2;
Eventloop<Epoll> loop;
int count(3);

void print1(int val);
void print2(int val);

void print1(int val)
{
  LOG_INFO << "print: " << val;

  if (--count == 0) 
  {
    timer1.stop();

    timer2.schedule(boost::bind(&print2, 2), 2000l, 0);
    timer2.start(&loop);
  }
}
void print2(int val)
{
  LOG_INFO << "print: " << val;

  if (count++ == 5)
  {
    timer2.stop();
    loop.stop();
  }
  else 
  {
    timer2.schedule(boost::bind(&print2, 1), 500l, 0);
  }
}

int main( )
{

  timer1.schedule(boost::bind(&print1, 3), 3000l, 1000l);
  timer1.start(&loop);

  loop.loop();
  // loop.join(); //already exited

  return 1;
}

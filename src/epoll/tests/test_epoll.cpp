
#include <stdio.h>
#include <unistd.h>
#include <boost/bind/bind.hpp>
#include <epoll/epoll.hpp> 
#include <epoll/loop.hpp> 
#include <epoll/timer.hpp> 

using namespace learn_cpp;

void print(int events, Timer *timer, int count) 
{
  ::printf("wait 2 secs, events: %d, count: %d\n", events, count);
  if (count<3) 
  {
    timer->timeout(boost::bind(&print, boost::placeholders::_1, timer, ++count), 2000);
  }
  else 
  {
    throw "stop loop";
  }
}

int main(int argc, const char * argv[]) {
  Timer timer;
  print(0, &timer, 0);

  {
    Eventloop<Epoll> loop;
    Epoll *epp = loop.getPoll();
    epp->ctlAdd(timer.getChan(), EPOLLIN);
    loop.loop();
  }

}
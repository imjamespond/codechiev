
#include <stdio.h>
#include <unistd.h>
#include <boost/bind/bind.hpp>
#include <epoll/epoll.hpp> 
#include <epoll/loop.hpp> 
#include <epoll/timer.hpp> 

using namespace learn_cpp;

void print(const ChannelPtr &chan,int events, Timer *timer, int count) 
{
  ::printf("wait 2 secs, events: %d, count: %d\n", events, count);
  if (count<3) 
  {
    timer->timeout(boost::bind(&print, boost::placeholders::_1, boost::placeholders::_2, timer, ++count), 2000);
  }
  else 
  {
    throw "stop loop";
  }
}

int main(int argc, const char * argv[]) {
  Timer timer;
  timer.timeout(boost::bind(&print, boost::placeholders::_1, boost::placeholders::_2, &timer, 0), 2000);

  {
    EventLoop<Epoll> loop;
    Epoll *epp = loop.GetPoll();
    epp->CtlAdd(timer.GetChan(), EPOLLIN);
    loop.Loop();
  }

}
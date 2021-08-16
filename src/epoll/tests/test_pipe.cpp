
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <boost/bind/bind.hpp> 
#include <epoll/epoll.hpp> 
#include <epoll/loop.hpp> 
#include <epoll/timer.hpp> 
#include <epoll/pipe.hpp> 
#include <utils/log.hpp> 

using namespace learn_cpp;
using namespace boost::placeholders;


Pipe pp;

bool onRead(ChannelPtr chan0, const char * buffer, int len) 
{ 
  Log() << buffer << ", len: " << len;
  return false;
}
void print() {
  while(1)
  {
    std::string input;
    std::cin >> input;
    if (input == "q") {  
      break;
    } else {
      ::close(pp.getChan0()->GetFd());
      ::write(pp.getChan1()->GetFd(), input.c_str(), input.length()); // automatically open fd0
    }
  }
} 


int main(int argc, const char * argv[]) {
  pp.onRead = boost::bind(&onRead, _1,_2,_3);
 
  Thread t1(print);

  {
    Eventloop<Epoll> loop;
    Epoll *ep = loop.getPoll();
    ep->CtlAdd(pp.getChan0());
    ep->CtlAdd(pp.getChan1());
    loop.loop();
  }

  t1.join();
}
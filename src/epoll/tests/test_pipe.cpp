
#include <stdio.h>
#include <unistd.h>
#include <boost/bind/bind.hpp>
#include <epoll/epoll.hpp> 
#include <epoll/loop.hpp> 
#include <epoll/timer.hpp> 
#include <epoll/pipe.hpp> 

using namespace learn_cpp;


int main(int argc, const char * argv[]) {
  Pipe pp;

  {
    Eventloop<Epoll> loop;
    Epoll *epp = loop.getPoll();
    epp->ctlAdd(pp.getChan0());
    epp->ctlAdd(pp.getChan1());
    loop.loop();
  }

}
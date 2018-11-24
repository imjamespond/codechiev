#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <errno.h> 
// #include <signal.h>
#include <boost/bind.hpp>

#include <network/Pipe.hpp>
#include <network/TcpServer.hpp>

#include <base/Time.hpp>
#include <base/Logger.hpp> 

using namespace codechiev::net;
using namespace codechiev::base;
 
 
int main( )
{
  Eventloop<Epoll> loop;  
  TcpServer serv(12345);
  serv.start(&loop);

  // TODO add handler to Channel and Epoll
  Pipe pipe;
  pipe.start(&loop);

  Time::SleepMillis(500l);

  pipe.write(0);

  return 1;
}

#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <errno.h> 
// #include <signal.h>
#include <boost/bind.hpp>

#include <network/Pipe.hpp>  
 
#include <base/Logger.hpp> 

using namespace codechiev::net;
using namespace codechiev::base;
 
 
int main( )
{
  Pipe<int> pipe();
  return 1;
}

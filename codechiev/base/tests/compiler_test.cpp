#include <stdio.h>
#include <boost/config.hpp>

int main(int argc, const char *argv[])
{
  #ifdef BOOST_NO_CXX11_LAMBDAS
    printf("boost_no_cxx11_lambdas\n");
  #endif
  return 0;
}
  
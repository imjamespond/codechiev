#include <stdio.h>
#include <stdint.h>
#include <boost/config.hpp>

int main(int argc, const char *argv[])
{
#ifdef BOOST_NO_CXX11_LAMBDAS
  printf("boost_no_cxx11_lambdas\n");
#endif

  printf("int: %lu bytes\n", sizeof(int));
  printf("short: %lu bytes\n", sizeof(short));
  printf("long: %lu bytes\n", sizeof(long));
  printf("long int: %lu bytes\n", sizeof(long int));
  printf("unsigned int: %lu bytes\n", sizeof(unsigned int));
  printf("unsigned short: %lu bytes\n", sizeof(unsigned short));
  printf("unsigned long: %lu bytes\n", sizeof(unsigned long));
  printf("long unsigned int: %lu bytes\n", sizeof(long unsigned int));
  printf("float: %lu bytes\n", sizeof(float));
  printf("double: %lu bytes\n", sizeof(double));
  printf("bool: %lu bytes\n", sizeof(bool));
  printf("char: %lu bytes\n", sizeof(char));
  printf("wchar_t: %lu bytes\n", sizeof(wchar_t));
  
  printf("int64_t: %lu bytes\n", sizeof(int64_t));
  printf("int32_t: %lu bytes\n", sizeof(int32_t));

  printf("void *: %lu bytes\n", sizeof(void *));
  return 0;
}

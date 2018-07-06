#include <stdio.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

void foobar(const char*, int, double, bool);
typedef boost::function<void()> foobar_func_1;
typedef boost::function<void(const char*)> foobar_func_2;
typedef boost::function<void(const char*, int, bool, double)> foobar_func_3;
typedef boost::function<void(int, const char*)> foobar_func_4;

int main ()
{ 
  foobar_func_1 func1 = boost::bind(foobar, "first func", 1, .111, true);
  func1();
  foobar_func_2 func2 = boost::bind(foobar, _1, 2, .222, true);
  func2("second func");
  foobar_func_3 func3 = boost::bind(foobar, _1, _2, _4, _3);
  func3("third func", 3, true, .333);
  foobar_func_4 func4 = boost::bind(foobar, _2, _1, .444, _1);
  func4(0, "forth func");
  return 0;
}

void foobar(const char* str, int i, double d, bool b)
{
  printf("%s, %d, %f, %d\n", str, i, d, b);
}
#include <stdio.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/unordered_map.hpp> 

#include <base/Logger.hpp> 
#include <base/Random.hpp> 
#include <base/Time.hpp>

void foobar(const char*, int, double, bool);
typedef boost::function<void()> foobar_func_1;
typedef boost::function<void(const char*)> foobar_func_2;
typedef boost::function<void(const char*, int, bool, double)> foobar_func_3;
typedef boost::function<void(int, const char*)> foobar_func_4;

int main ()
{ 
  //functor
  foobar_func_1 func1 = boost::bind(foobar, "first func", 1, .111, true);
  func1();
  foobar_func_2 func2 = boost::bind(foobar, _1, 2, .222, true);
  func2("second func");
  foobar_func_3 func3 = boost::bind(foobar, _1, _2, _4, _3);
  func3("third func", 3, true, .333);
  foobar_func_4 func4 = boost::bind(foobar, _2, _1, .444, _1);
  func4(0, "forth func");

  //random
  //unordered map 
  codechiev::base::Time begin = codechiev::base::Time::Now();
  boost::unordered_map<int, int> map;
  for(int i(0); i<99999; i++)
  {
    int rand = codechiev::base::random(0,1000000);
    map[rand] = rand;
  } 
  codechiev::base::Time now = codechiev::base::Time::Now();
  LOG_INFO<<(int)map.size()<<", "<<(now - begin);

  begin = codechiev::base::Time::Now();
  for(int i(0); i<99999; i++)
  {
    codechiev::base::GetRandom<int>();
  }
  now = codechiev::base::Time::Now();
  int rand = codechiev::base::GetRandom<int>();
  LOG_INFO << rand << ", " << (now - begin);

  return 0;
}

void foobar(const char* str, int i, double d, bool b)
{
  printf("%s, %d, %f, %d\n", str, i, d, b);
}
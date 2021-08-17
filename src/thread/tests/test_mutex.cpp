#include <thread/thread.hpp>
#include <thread/mutex.hpp>
#include <utils/log.hpp>

#include <stdio.h>
#include <unistd.h>

using namespace learn_cpp;

Mutex mutex;
int count = 0;

void print()
{
  Log() << "print";
  for (int i = 0; i < 10000; i++)
  {
    MutexGuard mg(&mutex);
    count++;
  }
  Log() << count;
}

int main(int argc, const char *argv[])
{
  Thread t1(print);
  Thread t2(print);
  Thread t3(print);
  t1.start();
  t2.start();
  t3.start();

  t1.join();
  t2.join();
  t3.join();

}
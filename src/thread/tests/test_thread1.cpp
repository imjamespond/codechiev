#include <thread/thread.hpp>

#include <stdio.h>
#include <unistd.h>

using namespace learn_cpp;

void print() {
  printf("print start\n");
  ::sleep(3);
  printf("print end\n");
}

int main(int argc, const char * argv[]) {
  Thread t1(print);
  printf("start\n");
  t1.start();
  printf("join\n");
  t1.join();
  printf("done\n");
}
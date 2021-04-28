#include <unistd.h> // pipe, close
#include <stdio.h>  // perror
#include <stdlib.h> // exit

#include <thread/thread.hpp>

#include <string>
#include <iostream>
#include <string.h> // memset,

int pipefd[2];
char buf[4];
void print();

int main(int argc, char *argv[])
{

  if (::pipe(pipefd) == -1)
  {
    ::perror("pipe");
    ::exit(EXIT_FAILURE);
  }

  learn_cpp::Thread thread(print);
  thread.start();

  std::string input;
  std::cin >> input;
  ::write(pipefd[1], input.c_str(), input.length());
  ::close(pipefd[1]); /* Reader will see EOF */

  ::printf("join\n");
  thread.join();
}

void print()
{

  ::printf("start read\n");
  ::memset(buf, 0, sizeof(buf));
  while (::read(pipefd[0], buf, sizeof(buf)-1) > 0)
  {
    ::printf("%s,", buf);
    ::memset(buf, 0, sizeof(buf));
  }
  ::close(pipefd[0]);
  ::printf("\nend\n");

  ::sleep(3);
}
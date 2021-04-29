#include <unistd.h> // pipe, close
#include <stdio.h>  // perror
#include <stdlib.h> // exit

#include <thread/thread.hpp>

#include <string>
#include <iostream>
#include <string.h> // memset,

int pipefd[2];
char buf[2];
const int bufLen = sizeof(buf);

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
  int len;
  // while ((len = ::read(pipefd[0], buf, bufLen)) > 0)
  while (1)
  {
    ::memset(buf, 0, bufLen);
    len = ::read(pipefd[0], buf, bufLen);
    if (len > 0)
    {
      std::string str(buf, buf + len);
      ::printf("%s$%d,", str.c_str(), len);
    }
    else
    {
      ::printf("close\n");
      ::close(pipefd[0]);
      break; // If nobody has the pipe open for writing, read() will always return 0 bytes and not block
    }
  }
  ::printf("end\n");

  ::sleep(3);
}
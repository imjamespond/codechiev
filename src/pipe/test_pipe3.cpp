#include <unistd.h> // pipe, close
#include <stdio.h> // perror
#include <stdlib.h> // exit

#include <thread/thread.hpp>

int main(int argc, char *argv[])
{
  int pipefd[2];

  if (::pipe(pipefd) == -1)
  {
    ::perror("pipe");
    ::exit(EXIT_FAILURE);
  }

}
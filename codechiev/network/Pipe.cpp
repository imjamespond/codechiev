#include "Pipe.hpp"

#include <unistd.h>
#include <base/Logger.hpp>

using namespace codechiev::net;

void __init_pipe(int *pipefd)
{
  // O_CLOEXEC - Specifying this flag permits a program to avoid additional
  // fcntl(2) F_SETFD operations to set the FD_CLOEXEC flag.
  if (-1 == ::pipe2(pipefd, O_NONBLOCK | O_CLOEXEC))
  {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
}

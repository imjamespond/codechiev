#include "Pipe.hpp"

#include <unistd.h>
#include <base/Logger.hpp>

using namespace codechiev::net;

void __init_pipe__(int *pipefd)
{
  // O_CLOEXEC - Specifying this flag permits a program to avoid additional
  // fcntl(2) F_SETFD operations to set the FD_CLOEXEC flag.
  if (-1 == ::pipe2(pipefd, O_NONBLOCK | O_CLOEXEC))
  {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
}

Pipe::Pipe()
{
  int pipefd[2];
  __init_pipe__(pipefd);
  p0 = new Channel(pipefd[0]);
  p1 = new Channel(pipefd[1]);
}

void Pipe::start(Eventloop<Epoll> *loop)
{
  loop->getPoll()->ctlAdd(p0);
  loop->getPoll()->ctlAdd(p1);
}

void Pipe::write(short pipe, void *data, ssize_t len)
{ 
  data = data == NULL ? &pipe : data;
  len = len ? len : sizeof pipe;
  if (pipe) 
  {
    ::write(p1->getFd(), data, len);
  }
  else 
  {
    ::write(p1->getFd(), data, len);
  }
}

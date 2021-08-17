#include <stdio.h>
#include <string>
#include <errno.h>
#include <boost/bind/bind.hpp>
#include "pipe.hpp"

using namespace learn_cpp;

Pipe::PipeFd::PipeFd()
{
  if (::pipe(fd) == -1)
  {
    ::perror("pipe");
    ::exit(EXIT_FAILURE);
  }
}

Pipe::Pipe(bool async) : pipe(), chan0(Channel::Create(pipe.fd[0])), chan1(Channel::Create(pipe.fd[1]))
{ 
}
Pipe::~Pipe()
{
  chan0->Close();
  chan1->Close();
  chan0->Reset();
  chan1->Reset();
}

int Pipe::ReadSync(char *buf, int bufLen)
{
  ::close(chan1->GetFd());
  return ::read(chan0->GetFd(), buf, bufLen); // automatically reopen fd1
}

void Pipe::WriteSync(const char *str, int len)
{
  ::close(chan0->GetFd());
  ::write(chan1->GetFd(), str, len); // automatically open fd0
}

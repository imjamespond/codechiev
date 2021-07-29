#include <stdio.h>
#include <string>
#include <errno.h>
#include <boost/bind/bind.hpp>
#include "pipe.hpp"
#include "./epoll.hpp"

using namespace learn_cpp;

Pipe::PipeFd::PipeFd()
{
  if (::pipe(fd) == -1)
  {
    ::perror("pipe");
    ::exit(EXIT_FAILURE);
  }
}

Pipe::Pipe(): pipe(), chan0(pipe.fd[0]), chan1(pipe.fd[1])
{
  this->chan0.func = boost::bind(&Pipe::readAsync, this, boost::placeholders::_1); 
  // this->chan1.func = boost::bind(&Pipe::handle, this, boost::placeholders::_1); 
}
Pipe::~Pipe()
{
  chan1.close();
}

int Pipe::readSync(char *buf, int bufLen)
{ 
	::close(chan1.getFd());
  return ::read(chan0.getFd(), buf, bufLen); // automatically reopen fd1
}

void Pipe::writeSync(const char * str, int len)
{
  ::close(chan0.getFd());
  ::write(chan1.getFd(), str, len); // automatically open fd0
}


void Pipe::readAsync(int events)
{ 
}

void Pipe::writeAsync(int events)
{
}
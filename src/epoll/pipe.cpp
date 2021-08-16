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

Pipe::Pipe(): pipe(), _chan0( Channel::Create(pipe.fd[0])), _chan1( Channel::Create(pipe.fd[1])), chan0(_chan0), chan1(_chan1)
{
  chan0->func = boost::bind(&Pipe::ReadAsync, this, boost::placeholders::_1); 
  //chan1.func = boost::bind(&Pipe::handle, this, boost::placeholders::_1); 
}
Pipe::~Pipe()
{
  chan0->Close();
  chan1->Close();
}

int Pipe::readSync(char *buf, int bufLen)
{ 
	::close(chan1->GetFd());
  return ::read(chan0->GetFd(), buf, bufLen); // automatically reopen fd1
}

void Pipe::writeSync(const char * str, int len)
{
  ::close(chan0->GetFd());
  ::write(chan1->GetFd(), str, len); // automatically open fd0
}


void Pipe::ReadAsync(int events)
{ 
  char buffer[1<<10];
  size_t buf_len = sizeof buffer;

  for (;;)
  {

    ::memset(buffer, 0, buf_len);
    ssize_t len = ::read(chan0->GetFd(), buffer, buf_len);

    if (len > 0 && onRead)
    {
      if (onRead(chan0, buffer, len)) 
      {
        break;
      } 
    }
    else if (-1 == len)
    {
      if (errno == EAGAIN && onReadEnd)
      {
        onReadEnd(chan0);
        break;
      }
    }
    else
    { 
      break;
    }
  }

  // flush(channel); //reading end and check if there is something to write in the channel!
}

void Pipe::WriteAsync(int events)
{
}
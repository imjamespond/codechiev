#include <stdio.h>
#include <string.h> // memset,
#include <sstream>
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

Pipe::Pipe(): pipe(), chan0(pipe.fd[0]), chan1(pipe.fd[1])
{
  this->chan0.func = boost::bind(&Pipe::readAsync, this, boost::placeholders::_1); 
  this->chan1.func = boost::bind(&Pipe::handle, this, boost::placeholders::_1); 
}
Pipe::~Pipe()
{
  chan1.close();
}

void Pipe::readSync()
{ 
  int len(0); 
  char buf[2];
  const int bufLen = sizeof(buf);
   
  while (1)
  {
    ::memset(buf, 0, bufLen);
    ::close(chan1.getFd());
    len = ::read(chan0.getFd(), buf, bufLen); // automatically reopen fd1
    if (len > 0)
    {
      std::string str(buf, buf + len);
      std::stringstream ss;
      ss << "buf: " << str.c_str() << ", len: " << len << std::endl;
      ::write(STDOUT_FILENO, ss.str().c_str(), ss.str().length());//写到标准输出, printf not work when its in fork child process
    } 
  }
}

void Pipe::writeSync(const char * str, int len)
{
  ::close(chan0.getFd());
  ::write(chan1.getFd(), str, len); // automatically open fd0
}


void Pipe::readAsync(int events)
{ 
  // if (events & EVENT_HUP_) 
  // if (events & EVENT_WRITE_)
  if (events & EVENT_READ_)
  {
    char buf[1<<10]; 
    for (;;)
    {
      ::memset(buf, 0, len);
      int len = ::read(this->chan0.getFd(), buf, sizeof buf);
      printf("read len: %d, %.*s\n", len, len, exp);

      if (-1 == len && errno == EAGAIN)
      {
        break;
      }
    }
  } 
}

void Pipe::writeAsync(int events)
{ 
  // if (events & EVENT_HUP_) 
  // if (events & EVENT_WRITE_)
  if (events & EVENT_READ_)
  {
    char buf[1<<10]; 
    for (;;)
    {
      ::memset(buf, 0, len);
      int len = ::read(this->chan0.getFd(), buf, sizeof buf);
      printf("read len: %d, %.*s\n", len, len, exp);

      if (-1 == len && errno == EAGAIN)
      {
        break;
      }
    }
  } 
}
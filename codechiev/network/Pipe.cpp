#include "Pipe.hpp"

#include <unistd.h>
#include <base/Logger.hpp>

using namespace codechiev::net;

void init_pipe__(int *pipefd)
{
    // O_CLOEXEC - Specifying this flag permits a program to avoid additional
    // fcntl(2) F_SETFD operations to set the FD_CLOEXEC flag.
    if (-1 == ::pipe2(pipefd, O_NONBLOCK | O_CLOEXEC))
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
}

Pipe::Pipe(): TcpEndpoint(true)
{
    int pipefd[2];
    init_pipe__(pipefd);
    p0 = Channel::CreateRaw(pipefd[0]);
    p1 = Channel::CreateRaw(pipefd[1]);

    p0->ptr = Channel::ChannelPtr(p0);
    p1->ptr = Channel::ChannelPtr(p1);
}

Pipe::~Pipe()
{
    p0->ptr.reset() ;
    p1->ptr.reset();
}

void Pipe::init(Channel::Loop *loop)
{
    loop->getPoll()->ctlAdd(p0);
    loop->getPoll()->ctlAdd(p1);

    p0->loop = loop;
    p1->loop = loop;

    Channel::Handler handler = boost::bind(&TcpEndpoint::handle, this, _1);
    p0->handler = handler;
    p1->handler = handler;
}

void Pipe::notify(const void *data, int len)
{
    // ::write(getFd(), data, len); 
    if (data == NULL) 
    {
        int msg(0);
        this->send(p1->ptr, (const char *)&msg, sizeof msg);
    }
    else 
    {
        this->send(p1->ptr, (const char *)data, len);
    }
}

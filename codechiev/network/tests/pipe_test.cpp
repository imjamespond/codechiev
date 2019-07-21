#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <errno.h>
// #include <signal.h>
#include <boost/bind.hpp>

#include <network/Pipe.hpp>

#include <base/Time.hpp>
#include <base/Logger.hpp>

using namespace codechiev::net;
using namespace codechiev::base;

typedef Channel::ChannelPtr ChannelPtr;

void onPipeRead(const ChannelPtr &);

int main()
{
    Pipe pipe;
    pipe.setEndReading(boost::bind(&onPipeRead, _1));

    {
        TcpEndpoint::Loop loop;

        pipe.start(&loop);

        Time::SleepMillis(500l);

        pipe.notify();
    }

    return 1;
}

void onPipeRead(const ChannelPtr &channel)
{
    LOG_INFO << " fd: " << channel->getFd();
    
    ((TcpEndpoint::Loop*)channel->loop)->stop();
}
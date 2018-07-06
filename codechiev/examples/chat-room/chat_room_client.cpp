#include <stdio.h>
#include <boost/bind.hpp>

#include <base/Logger.hpp>
#include <base/Time.hpp>
#include <base/Thread.hpp>
#include <base/Keyboard.hpp>
#include <libev/Channel.hpp>
#include <libev/TcpClient.hpp> 

using namespace codechiev::base;
using namespace codechiev::libev;

// typedef struct
// {
//     TcpClient *client;
//     TcpEndpoint::bufev_struct *bufev;
// } ChatRoomClient;
Channel *__client__(NULL);

int onMessage(const char* msg, int len, int fd)
{

    STREAM_INFO << msg;
    return 0;
}

int onClientConnect( Channel *channel)
{
    // STREAM_INFO;
    __client__ = channel;
    channel->onMessage = boost::bind(onMessage,_1,_2,_3);
    return 0;
}

int onClientClose( Channel *channel)
{
    // STREAM_INFO;
    delete channel;
    return 0;
}

int onClientRead( Channel *channel )
{
    struct evbuffer *evbuf = bufferevent_get_input(channel->bufev);
    int len = evbuffer_get_length(evbuf);
    unsigned char *data = evbuffer_pullup(evbuf, len);
    int has_read = channel->decode((const char *)data,len);//consider put it in multiple threads
    evbuffer_drain(evbuf, has_read);
    return 0;
}

int onClientWrite(Channel *channel)
{
    // LOG_INFO;
    return 0;
}

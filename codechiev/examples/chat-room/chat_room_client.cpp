#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp> 

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

typedef boost::shared_ptr<Channel> channel_ptr;
typedef boost::unordered_map<int, channel_ptr> ChannelMap;
ChannelMap __client_channels__;
long __client_write_bytes__ = 0;

int onMessage(const char* msg, int len, Channel *channel)
{ 
    std::string msg_str(msg, len);
    STREAM_INFO << msg_str;
    return 0;
}

int onClientConnect( Channel *channel)
{
    STREAM_TRACE;
    __client_channels__[channel->fd] = channel_ptr(channel);
    channel->onMessage = boost::bind(onMessage,_1,_2,_3);
    return 0;
}

int onClientClose( Channel *channel)
{
    STREAM_TRACE;
    __client_channels__.erase(channel->fd);
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
    __client_write_bytes__+=channel->write_bytes;
    return 0;
}

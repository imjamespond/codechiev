#include <stdio.h>

#include <base/Logger.hpp>
#include <base/Time.hpp>
#include <base/Thread.hpp>
#include <base/Keyboard.hpp>
#include <libev/Channel.hpp>
#include <libev/TcpClient.hpp> 

using namespace codechiev::base;
using namespace codechiev::libev;

typedef struct
{
    TcpClient *client;
    Channel::bufev_struct *bufev;
} ChatRoomClient;
ChatRoomClient __Client__;

int onClientConnect(TcpEndpoint *client, Channel::bufev_struct *bev)
{
    STREAM_INFO;
    __Client__.bufev = bev;
    return 0;
}

int onClientClose(TcpEndpoint *client, Channel::bufev_struct *bev)
{
    // STREAM_INFO;
    return 0;
}

int onClientRead(TcpEndpoint *client, Channel::bufev_struct *bev, void *data, int len)
{
    // std::string msg((char *)data, len);
    // LOG_INFO ; 

    // client->write(bev, msg.c_str(), len);//within recursive locks
    return 0;
}

int onClientWrite(TcpEndpoint *client, Channel::bufev_struct *bev)
{
    // LOG_INFO;
    return 0;
}

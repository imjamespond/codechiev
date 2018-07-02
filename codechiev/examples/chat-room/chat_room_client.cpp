#include <stdio.h>

#include <base/Logger.hpp>
#include <base/Time.hpp>
#include <base/Thread.hpp>
#include <base/Keyboard.hpp>
#include <libev/TcpClient.hpp> 

using namespace codechiev::base;
using namespace codechiev::libev;

TcpClient *Cli(NULL);
TcpClient::bufferevent_struct *BufEv(NULL);

int onClientConnect(TcpEndpoint *client, TcpEndpoint::bufferevent_struct *bev)
{
    // LOG_INFO << "";
    Cli = static_cast<TcpClient *> (client);
    BufEv = bev;
    return 0;
}

int onClientClose(TcpEndpoint *client, TcpEndpoint::bufferevent_struct *bev)
{
    LOG_INFO;
    return 0;
}

int onClientRead(TcpEndpoint *client, TcpEndpoint::bufferevent_struct *bev, void *data, int len)
{
    // std::string msg((char *)data, len);
    // LOG_INFO << "read:" << len << "," << msg; 

    // client->write(bev, msg.c_str(), len);//within recursive locks
    return 0;
}

int onClientWrite(TcpEndpoint *client, TcpEndpoint::bufferevent_struct *bev)
{
    // LOG_INFO << "";
    return 0;
}

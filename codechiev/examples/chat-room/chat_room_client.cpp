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
    // STREAM_INFO;
    // Cli = static_cast<TcpClient *> (client);
    // BufEv = bev;

    const char msg[] = "welcome to visit";
    int head(sizeof msg);
    TcpEndpoint::Write(bev, reinterpret_cast<const char *>(&head), sizeof(int));
    TcpEndpoint::Write(bev, msg, head);
    return 0;
}

int onClientClose(TcpEndpoint *client, TcpEndpoint::bufferevent_struct *bev)
{
    // STREAM_INFO;
    return 0;
}

int onClientRead(TcpEndpoint *client, TcpEndpoint::bufferevent_struct *bev, void *data, int len)
{
    // std::string msg((char *)data, len);
    // LOG_INFO ; 

    // client->write(bev, msg.c_str(), len);//within recursive locks
    return 0;
}

int onClientWrite(TcpEndpoint *client, TcpEndpoint::bufferevent_struct *bev)
{
    // LOG_INFO;
    return 0;
}

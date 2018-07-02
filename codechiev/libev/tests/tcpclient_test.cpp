#include <stdio.h>

#include <base/Logger.hpp>
#include <base/Time.hpp>
#include <base/Thread.hpp>
#include <base/Keyboard.hpp>
#include <libev/TcpClient.hpp> 

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

using namespace codechiev::base;
using namespace codechiev::libev;

TcpClient *Cli(NULL);
TcpClient::bufferevent_struct *BufEv(NULL);

int onConnect(TcpClient *cli, TcpClient::bufferevent_struct *bev)
{
    // LOG_INFO << "";
    Cli = cli;
    BufEv = bev;
    return 0;
}

int onClose(TcpClient *client, TcpClient::bufferevent_struct *bev)
{
    // LOG_INFO;
    return 0;
}

int onRead(TcpClient *client, TcpClient::bufferevent_struct *bev, void *data, int len)
{
    // std::string msg((char *)data, len);
    // LOG_INFO << "read:" << len << "," << msg; 

    // client->write(bev, msg.c_str(), len);//within recursive locks
    return 0;
}

int onWrite(TcpClient *server, TcpClient::bufferevent_struct *bev)
{
    // LOG_INFO << "";
    return 0;
}

void run_client(int argc, const char * argv[])
{
    const char * hostname = argc>2 ? argv[2] : "127.0.0.1:12345";
    TcpClient client(hostname);

    client.onConnect = boost::bind(&onConnect, &client, _1);
    client.onClose = boost::bind(&onClose, &client, _1);
    client.onRead = boost::bind(&onRead, &client, _1, _2, _3);
    client.onWrite = boost::bind(&onWrite, &client, _1);

    int num = argc>1 ? boost::lexical_cast<int>(argv[1]) : 10;
    for(int i=0; i<num; i++)
    {
        client.connect();
    }
    client.start(); 
}

int main(int argc, const char * argv[]) {

    Thread threadCli("cli", boost::bind(&run_client, argc, argv));
    threadCli.start();

    while(1)
    {
        int input = keyboard::getchar();
        if(Cli && BufEv)
        {
            Cli->write(BufEv, reinterpret_cast<const char *>(&input), sizeof input);
        }
    }


    threadCli.join();
    return 0;
}

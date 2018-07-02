#include <base/Thread.hpp>
#include <base/Condition.hpp>
#include <base/Keyboard.hpp>
#include <libev/TcpServer.hpp>
#include <libev/TcpClient.hpp>

#include <event2/event.h>
#include <event2/event_struct.h>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "chat_room_server.hpp"

using namespace codechiev::base;
using namespace codechiev::libev;

CountLatch Latch;

int onServAccept(TcpServer *serv, TcpEndpoint::bufferevent_struct *bev);
int onServClose(TcpEndpoint *endpoint, TcpEndpoint::bufferevent_struct *bev);
int onServRead(TcpEndpoint *endpoint, TcpEndpoint::bufferevent_struct *bufev, void *data, int len);
int onServWrite(TcpEndpoint *endpoint, TcpEndpoint::bufferevent_struct *bev);

int onClientConnect(TcpEndpoint *client, TcpEndpoint::bufferevent_struct *bev);
int onClientClose(TcpEndpoint *client, TcpEndpoint::bufferevent_struct *bev);
int onClientRead(TcpEndpoint *client, TcpEndpoint::bufferevent_struct *bev, void *data, int len);
int onClientWrite(TcpEndpoint *client, TcpEndpoint::bufferevent_struct *bev);

ChatRoomServer *ServerPtr;
TcpClient *CliPtr;

void run_server( )
{
    ChatRoomServer server;
    ServerPtr = &server;

    server.onAccept = boost::bind(&onServAccept, &server, _1);
    server.onClose = boost::bind(&onServClose, &server, _1);
    server.onRead = boost::bind(&onServRead, &server, _1, _2, _3);
    server.onWrite = boost::bind(&onServWrite, &server, _1);

    Latch.unlatch();

    server.bind();
}

void run_client(int argc, const char *argv[])
{
    const char *hostname = argc > 2 ? argv[2] : "127.0.0.1:12345";
    TcpClient client(hostname);
    CliPtr = &client;

    client.onConnect = boost::bind(&onClientConnect, &client, _1);
    client.onClose = boost::bind(&onClientClose, &client, _1);
    client.onRead = boost::bind(&onClientRead, &client, _1, _2, _3);
    client.onWrite = boost::bind(&onClientWrite, &client, _1);

    int num = argc > 1 ? boost::lexical_cast<int>(argv[1]) : 10;
    for (int i = 0; i < num; i++)
    {
        client.connect();
    }
    client.start();
}

int main(int argc, const char *argv[])
{ 
    Thread serverThread("server", boost::bind(&run_server));
    serverThread.start();

    Latch.latch();

    Thread cliThread("cli", boost::bind(&run_client, argc, argv));
    cliThread.start();

    while (1)
    {
        // int input = keyboard::getchar();
        // if (Cli && BufEv)
        // {
        //     Cli->write(BufEv, reinterpret_cast<const char *>(&input), sizeof input);
        // }

        char buffer[32];
        if (keyboard::fgets(buffer, 32) != NULL)
        {
            if (0 == strcmp(buffer, "total\n"))
            { 
                printf("display total connections: %d\n", ServerPtr->totalClient());
            }
            else
            {
                ServerPtr->stop();
                CliPtr->stop();
                break;
            }
        }
    }

    printf("break");

    serverThread.join();
    cliThread.join();

    return 0;
} 
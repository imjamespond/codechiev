#include <base/Thread.hpp>
#include <base/Condition.hpp>
#include <base/Keyboard.hpp>
#include <base/Time.hpp>
#include <libev/Timer.hpp>
#include <libev/TcpClient.hpp>
#include <libev/Channel.hpp>

#include <event2/event.h>
#include <event2/event_struct.h>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "chat_room_server.hpp"

using namespace codechiev::base;
using namespace codechiev::libev;

CountLatch Latch;

int onServAccept(TcpServer *serv, Channel::bufev_struct *bev);
int onServClose(TcpEndpoint *endpoint, Channel::bufev_struct *bev);
int onServRead(TcpEndpoint *endpoint, Channel::bufev_struct *bufev, void *data, int len);
int onServWrite(TcpEndpoint *endpoint, Channel::bufev_struct *bev);

int onClientConnect(TcpEndpoint *client, Channel::bufev_struct *bev);
int onClientClose(TcpEndpoint *client, Channel::bufev_struct *bev);
int onClientRead(TcpEndpoint *client, Channel::bufev_struct *bev, void *data, int len);
int onClientWrite(TcpEndpoint *client, Channel::bufev_struct *bev);

ChatRoomServer *ServerPtr;

typedef struct
{
    TcpClient *client;
    Channel::bufev_struct *bufev;
} ChatRoomClient;
extern ChatRoomClient __Client__;

void on_server_run();
void run_server()
{
    ChatRoomServer server;
    ServerPtr = &server;

    server.onAccept = boost::bind(&onServAccept, &server, _1);
    server.onClose = boost::bind(&onServClose, &server, _1);
    server.onRead = boost::bind(&onServRead, &server, _1, _2, _3);
    server.onWrite = boost::bind(&onServWrite, &server, _1);

    Timer timer(server.base);
    timer.timeout(boost::bind(&on_server_run),0,500); 

    server.bind();
}

void run_client(int argc, const char *argv[])
{
    const char *hostname = argc > 2 ? argv[2] : "127.0.0.1:12345";
    TcpClient client(hostname);
    __Client__.client = &client;

    client.onConnect = boost::bind(&onClientConnect, &client, _1);
    client.onClose = boost::bind(&onClientClose, &client, _1);
    client.onRead = boost::bind(&onClientRead, &client, _1, _2, _3);
    client.onWrite = boost::bind(&onClientWrite, &client, _1);

    int num = argc > 1 ? boost::lexical_cast<int>(argv[1]) : 1;
    for (int i = 0; i < num; i++)
    {
        client.connect();
    }
    client.start();
}

void on_server_run()
{
    STREAM_INFO << "server now is running.";
    Latch.unlatch();
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

        char buffer[32];
        if (keyboard::fgets(buffer, 32) != NULL)
        {
            if (0 == strcmp(buffer, "total\n"))
            { 
                printf("display total connections: %d\n", ServerPtr->totalClient());
            }
            if (0 == strcmp(buffer, "test\n"))
            {
                Channel channel(__Client__.bufev);

                const char msg[] = "welcome to visit";
                const char *encoded = channel.encode(msg);
                channel.encode(msg);
                channel.encode(msg);
                // STREAM_INFO << encoded+4;
                int sendBufSize = channel.sendBufSize();
                int count(0),len(0);
                
                len = 2;
                printf("sending %d byte\n", len);
                TcpEndpoint::Write(__Client__.bufev, encoded+count, len);
                count+=2; 
                keyboard::getchar();
                

                len = 10;
                printf("sending %d byte\n", len);
                TcpEndpoint::Write(__Client__.bufev, encoded+count, len);
                count+=10; 
                keyboard::getchar();

                int left(sendBufSize-count);
                printf("sending %d byte\n", left);
                TcpEndpoint::Write(__Client__.bufev, encoded+count, left); 
            }
            else
            {
                ServerPtr->stop();
                __Client__.client->stop();
                break;
            }
        }
    }

    LOG_INFO << "server exit.";

    serverThread.join();
    cliThread.join();

    return 0;
} 
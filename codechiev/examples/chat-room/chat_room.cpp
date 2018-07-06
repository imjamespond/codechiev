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

int onServAccept(Channel*);
int onServClose(Channel*);
int onServRead(Channel*);
int onServWrite(Channel*);

int onClientConnect(Channel*);
int onClientClose(Channel*);
int onClientRead(Channel*);
int onClientWrite(Channel*);

ChatRoomServer *__server_ptr__;

typedef struct
{
    TcpClient *client;
    TcpEndpoint::bufev_struct *bufev;
} ChatRoomClient;
extern ChatRoomClient __client__;

void on_server_run();
void run_server()
{
    ChatRoomServer server;
    __server_ptr__ = &server;

    server.onAccept = boost::bind(&onServAccept, _1);
    server.onClose = boost::bind(&onServClose, _1);
    server.onRead = boost::bind(&onServRead, _1);
    server.onWrite = boost::bind(&onServWrite, _1);

    Timer timer(server.base);
    timer.timeout(boost::bind(&on_server_run),0,500); 

    server.bind();
}

void run_client(int argc, const char *argv[])
{
    const char *hostname = argc > 2 ? argv[2] : "127.0.0.1:12345";
    TcpClient client(hostname);
    __client__.client = &client;

    client.onConnect = boost::bind(&onClientConnect, _1);
    client.onClose = boost::bind(&onClientClose, _1);
    client.onRead = boost::bind(&onClientRead, _1);
    client.onWrite = boost::bind(&onClientWrite, _1);

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

void test_1();

int main(int argc, const char *argv[])
{ 
    Thread serverThread("server", boost::bind(&run_server));
    serverThread.start();

    Latch.latch();

    Thread cliThread("cli", boost::bind(&run_client, argc, argv));
    cliThread.start();

    int code = 0;
    do
    {
        code = keyboard::getchar();
        if(code == keycode::a)
        {
            test_1();
        }
        else if(code == keycode::b)
        {
            __server_ptr__->broadcast("hello all!");
        }
        else if(code == keycode::c)
        {
            printf("command:");

            char buffer[32] = {0};
            keyboard::fscanf(buffer); 
            printf("\nexecute: %s\n", buffer);
            if (0 == strcmp(buffer, "total"))
            { 
                printf("display total connections: %d\n", __server_ptr__->totalClient());
            }
            else if (0 == strcmp(buffer, "stop"))
            {
                __server_ptr__->stop();
                __client__.client->stop();
                break;
            } 
        }
    }while(1);

    LOG_INFO << "server exit.";

    serverThread.join();
    cliThread.join();

    return 0;
} 

void test_1()
{
    Channel channel(__client__.client, __client__.bufev);

    const char msg[] = "welcome to chatroom";
    const char *encoded = channel.encode(msg);
    channel.encode(msg);
    channel.encode(msg);
    // STREAM_INFO << encoded+4;
    int sendBufSize = channel.sendBufSize();
    int count(0),len(0);
    
    len = 2;
    printf("sending %d byte\n", len);
    TcpEndpoint::Write(__client__.bufev, encoded+count, len);
    count+=2; 
    Time::SleepMillis(500l);
    

    len = 10;
    printf("sending %d byte\n", len);
    TcpEndpoint::Write(__client__.bufev, encoded+count, len);
    count+=10; 
    Time::SleepMillis(500l);

    int left(sendBufSize-count);
    printf("sending %d byte\n", left);
    TcpEndpoint::Write(__client__.bufev, encoded+count, left); 
}
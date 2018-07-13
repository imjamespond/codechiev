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

int onServAccept(Channel *);
int onServClose(Channel *);
int onServRead(Channel *);
int onServWrite(Channel *);

int onClientConnect(Channel *);
int onClientClose(Channel *);
int onClientRead(Channel *);
int onClientWrite(Channel *);

ChatRoomServer *__server_ptr__;
TcpClient *__client__;

typedef boost::shared_ptr<Channel> channel_ptr;
typedef boost::unordered_map<int, channel_ptr> ChannelMap;
extern ChannelMap __client_channels__;

struct event *active_ev;

void on_server_run(int, short, void *);
void on_server_statics(ChatRoomServer *);
void run_server()
{
    ChatRoomServer server;
    __server_ptr__ = &server;

    server.onAccept = boost::bind(&onServAccept, _1);
    server.onClose = boost::bind(&onServClose, _1);
    server.onRead = boost::bind(&onServRead, _1);
    server.onWrite = boost::bind(&onServWrite, _1);

    active_ev = event_new(server.base,
                          -1,
                          EV_READ | EV_PERSIST /*| EV_ET will probally cause connection failed*/,
                          on_server_run,
                          NULL);
    event_add(active_ev, NULL);

    // Timer statics(server.base, EV_PERSIST);
    // statics.timeout(boost::bind(&on_server_statics, &server), 10);
    Latch.unlatch();
    server.bind();
}

void run_client(int argc, const char *argv[])
{
    Latch.latch(); //wait for activeev set
    event_active(active_ev, 0, 0);
    Latch.latch(); //wait for server started

    const char *hostname = argc > 2 ? argv[2] : "127.0.0.1:12345";
    TcpClient client(hostname);
    __client__ = &client;

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

void on_server_run(int fd, short flags, void *data)
{
    STREAM_INFO << "server now is running.";
    Latch.unlatch();
}

void on_server_statics(ChatRoomServer *server)
{
    LOG_INFO_R << " count:" << server->getCount();
}

int test_1();
int test_2();

int main(int argc, const char *argv[])
{
    Thread serverThread("server", boost::bind(&run_server));
    serverThread.start();

    Thread cliThread("cli", boost::bind(&run_client, argc, argv));
    cliThread.start();

    int code = 0;
    do
    {
        code = keyboard::getchar();
        if (code == keycode::a)
        {
            // __client__ && test_1();
        }
        else if (code == keycode::num2)
        {
            test_2();
        }
        else if (code == keycode::b)
        {
            __server_ptr__->broadcast("hello all!");
        }
        else if (code == keycode::c)
        {
            printf("command:");
            char buffer[32] = {0};
            keyboard::fscanf(buffer);
            printf("\nexecute: %s\n", buffer);

            if (0 == strcmp(buffer, "total"))
            {
                printf("display total connections: %d\n", __server_ptr__->totalClient());
            }
            else if (0 == strcmp(buffer, "count"))
            {
                printf("count: %d\n", __server_ptr__->getCount());
            }
            else if (0 == strcmp(buffer, "stop"))
            {
                __server_ptr__->stop();
                // __client__ && __client__->endpoint->stop();
                break;
            }
        }
    } while (1);

    LOG_INFO << "server exit.";

    serverThread.join();
    cliThread.join();

    return 0;
}

// int test_1()
// {
//     const char msg[] = "welcome to chatroom";
//     const char *encoded = __client__->encode(msg);
//     __client__->encode(msg);
//     __client__->encode(msg);
//     // STREAM_INFO << encoded+4;
//     int sendBufSize = __client__->getSendBufSize();
//     int count(0),len(0);

//     len = 2;
//     printf("sending %d byte\n", len);
//     TcpEndpoint::Write(__client__->bufev, encoded+count, len);
//     count+=2;
//     Time::SleepMillis(500l);

//     len = 10;
//     printf("sending %d byte\n", len);
//     TcpEndpoint::Write(__client__->bufev, encoded+count, len);
//     count+=10;
//     Time::SleepMillis(500l);

//     int left(sendBufSize-count);
//     printf("sending %d byte\n", left);
//     TcpEndpoint::Write(__client__->bufev, encoded+count, left);

//     __client__->setSendBufSize(0);
//     return 0;
// }

int test_2()
{
    printf("input msg: \n");
    char msg[32] = {0};
    keyboard::fscanf(msg);
    printf("msg: %s\n", msg);
    ChannelMap::iterator it;
    for (it = __client_channels__.begin(); it != __client_channels__.end(); ++it)
    {
        channel_ptr channel = it->second;
        if (channel)
        {
            channel->send(msg);
        }
        else
        {
            LOG_TRACE << "bufferevent is null";
        }
    }
    return 0;
}
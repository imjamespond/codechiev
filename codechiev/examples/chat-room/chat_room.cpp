#include <base/Thread.hpp>
#include <base/Keyboard.hpp>
#include <libev/TcpServer.hpp>

#include <event2/event.h>
#include <event2/event_struct.h>

#include <boost/bind.hpp>

#include "chat_room_server.h"

using namespace codechiev::base;
using namespace codechiev::libev;

int onServAccept(TcpServer *serv, TcpServer::bufferevent_struct *bev);
int onServClose(TcpEndpoint *endpoint, TcpEndpoint::bufferevent_struct *bev);
int onServRead(TcpEndpoint *endpoint, TcpEndpoint::bufferevent_struct *bufev, void *data, int len);
int onServWrite(TcpEndpoint *endpoint, TcpEndpoint::bufferevent_struct *bev);


using codechiev::base::keyboard;
void read_stdin(int fd, short flags, void *data)
{
    char buffer[32];
    if ( keyboard::fgets (buffer , 32) != NULL )
    { 
        if(0 == strcmp(buffer, "total\n"))
        {
            ChatRoomServer *server = reinterpret_cast<ChatRoomServer *>(data);
            printf("display total connections: %d\n", server->totalClient());
        }
    } 

    return;
} 

void run_server( )
{
    ChatRoomServer server;

    server.onAccept = boost::bind(&onServAccept, &server, _1);
    server.onClose = boost::bind(&onServClose, &server, _1);
    server.onRead = boost::bind(&onServRead, &server, _1, _2, _3);
    server.onWrite = boost::bind(&onServWrite, &server, _1);

    int inputfd = fileno (stdin);
    struct event inputev;
    event_assign(&inputev, 
                server.base, 
                inputfd /*stdin*/, 
                EV_READ | EV_PERSIST, 
                read_stdin, 
                &server
    );
    event_add(&inputev, NULL);

    server.bind();
}

int main(int argc, char **argv)
{ 
    Thread serverThread("server", boost::bind(&run_server));
    serverThread.start();
    serverThread.join();

    return 0;
} 
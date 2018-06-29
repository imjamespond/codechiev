#include <libev/TcpServer.hpp>
#include <libev/Channel.hpp>
#include <libev/Signal.hpp>
#include <base/Mutex.hpp>
#include <base/Thread.hpp>
#include <base/Logger.hpp>
#include <base/Keyboard.hpp>

#include <boost/bind.hpp>
#include <boost/unordered_map.hpp>
#include <event2/event_struct.h>

using namespace codechiev::base;
using namespace codechiev::libev;

class ChatRoomServer : public TcpServer
{
public:
    ChatRoomServer();

    void broadcast(const char *);
    int totalClient();

private:
    typedef boost::unordered_map<std::string, bufferevent_struct *> BuffereventMap;
    BuffereventMap clients;

    Mutex::mutex_ptr mutexCli;

};


int onAccept(TcpServer *serv, TcpServer::bufferevent_struct *bev)
{
    // ChatRoomServer *server = static_cast<ChatRoomServer *>(serv);
    STREAM_INFO;
    return 0;
}

int onClose(TcpEndpoint *endpoint, TcpEndpoint::bufferevent_struct *bev)
{
    // ChatRoomServer *server = static_cast<ChatRoomServer *>(endpoint);
    STREAM_INFO;
    return 0;
}

int onRead(TcpEndpoint *endpoint, TcpEndpoint::bufferevent_struct *bufev, void *data, int len)
{
    // std::string msg((char *)data, len);
    struct evbuffer *evbuf = bufferevent_get_input(bufev);
    Channel::Decode(evbuf);
    return 0;
}

int onWrite(TcpEndpoint *endpoint, TcpEndpoint::bufferevent_struct *bev)
{
    STREAM_INFO;
    return 0;
}

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

    server.onAccept = boost::bind(&onAccept, &server, _1);
    server.onClose = boost::bind(&onClose, &server, _1);
    server.onRead = boost::bind(&onRead, &server, _1, _2, _3);
    server.onWrite = boost::bind(&onWrite, &server, _1);

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

ChatRoomServer::ChatRoomServer() : TcpServer(12345)
{
    mutexCli = Mutex::mutex_ptr(new Mutex);
}

void 
ChatRoomServer::broadcast(const char * msg)
{
  //TODO must lock buffer, lock bevMap
  BuffereventMap::iterator it;
  for (it = clients.begin(); it != clients.end(); ++it)
  {
    bufferevent_struct *bev = it->second;

    if (bev) {
        write(bev, msg, ::strlen(msg)); 
    }
    else
    {
        LOG_TRACE << "bufferevent is null";
    }
    
  }
}

int 
ChatRoomServer::totalClient()
{
    MutexLock lock(mutexCli);
    return clients.size();
}
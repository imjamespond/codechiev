
#include <libev/TcpServer.hpp>
#include <libev/Signal.hpp>
#include <base/Logger.hpp>
#include <base/Keyboard.hpp>

#include <boost/bind.hpp>
#include <boost/unordered_map.hpp>
#include <event2/event_struct.h>

using namespace codechiev::base;
using namespace codechiev::libev; 

static void signal_cb(evutil_socket_t, short, void *);


class TcpServerExt : public TcpServer
{
    public:
    TcpServerExt() : TcpServer(12345), total(0)
    { }
    
    void broadcast(const char *);

    typedef boost::unordered_map<int, bufferevent_struct *> BuffereventMap;
    BuffereventMap bevMap;
    int total;
};


int onConnect(TcpServer *serv, TcpServer::bufferevent_struct *bev)
{
    // serv->bevMap[fd] = bev;
    // serv->broadcast("foobar");
    // serv->write(bev, "welcome to visit");
    
    TcpServerExt *servext = static_cast<TcpServerExt *>(serv);
    servext->total++;
    // LOG_INFO << "";
    return 0;
}

int onClose(TcpServer *server, TcpServer::bufferevent_struct *bev)
{
    // evutil_socket_t fd = bufferevent_getfd(bev);
    // server->bevMap.erase(fd);
    // LOG_TRACE << "buffer event map: " << (int)server->bevMap.size();
    TcpServerExt *servext = static_cast<TcpServerExt *>(server);
    servext->total--;
    // LOG_INFO << "";
    return 0;
}

int onRead(TcpServer *server, TcpServer::bufferevent_struct *bev, void *data, int len)
{
    std::string msg((char *)data, len);
    LOG_INFO << "read:" << len << "," << msg; 

    server->write(bev, msg.c_str(), len);//within recursive locks
    return 0;
}

int onWrite(TcpServer *server, TcpServer::bufferevent_struct *bev)
{
    LOG_INFO << "";
    return 0;
}

using codechiev::base::keyboard;
void read_stdin(int fd, short flags, void *data)
{
    char buffer[32];
    if ( keyboard::fgets (buffer , 32) != NULL )
    {
        // printf("fgets: %s\n",buffer);
        if(0 == strcmp(buffer, "total\n"))
        {
            TcpServerExt *server = reinterpret_cast<TcpServerExt *>(data);
            printf("display total connections: %d\n", server->total);
        }
    }

    // struct event *inputev = reinterpret_cast<struct event *>(data);
    // event_del(inputev);

    return;
}

int main(int argc, char **argv)
{

    TcpServerExt server;
    Signal signal(server.base, &signal_cb, &server);

    server.onConnect = boost::bind(&onConnect, &server, _1);
    server.onClose = boost::bind(&onClose, &server, _1);
    server.onRead = boost::bind(&onRead, &server, _1, _2, _3);
    server.onWrite = boost::bind(&onWrite, &server, _1);

    int inputfd = fileno (stdin);
    struct event inputev;
    event_assign(&inputev, 
                server.base, 
                inputfd /*stdin*/, 
                EV_READ | EV_PERSIST, 
                read_stdin, &server);
    event_add(&inputev, NULL);

    server.bind();

    return 0;
}


static void
signal_cb(evutil_socket_t sig, short events, void *user_data)
{
    TcpServer *serv = reinterpret_cast<TcpServer *>(user_data);
    // struct timeval delay = {2, 0};
    printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");
    // event_base_loopexit(base, &delay);
    serv->stop();
}


void 
TcpServerExt::broadcast(const char * msg)
{
  //TODO must lock buffer, lock bevMap
  BuffereventMap::iterator it;
  for (it = bevMap.begin(); it != bevMap.end(); ++it)
  {
    bufferevent_struct *bev = it->second;

    if (bev) {
        write(bev, msg, ::strlen(msg)); 
    }else{
        LOG_TRACE << "bufferevent is null";
    }
    
  }
}
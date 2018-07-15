
#include <libev/TcpServer.hpp>
#include <libev/Signal.hpp>
#include <base/Logger.hpp>
#include <base/Keyboard.hpp>
#include <base/Mutex.hpp> 

#include <boost/bind.hpp>
#include <boost/unordered_map.hpp>
#include <event2/event_struct.h>

using namespace codechiev::base;
using namespace codechiev::libev; 

static void signal_cb(evutil_socket_t, short, void *);
Mutex::mutex_ptr mutex = Mutex::mutex_ptr(new Mutex) ;
long recv_bytes = 0;

class TcpServerExt : public TcpServer
{
    public:
    TcpServerExt() : TcpServer(12345), total(0)
    { }
    
    void broadcast(const char *);

    typedef boost::unordered_map<int, TcpEndpoint::bufev_struct *> BuffereventMap;
    BuffereventMap bevMap;
    int total;
};

void onMessage(const char *msg, int len, Channel *channel)
{
    MutexLock lock(mutex);
    recv_bytes += len;
}

int onAccept( Channel *channel)
{ 
    channel->onMessage = boost::bind(&onMessage, _1, _2, _3);
    
    return 0;
}

int onClose( Channel *channel)
{
    // evutil_socket_t fd = bufferevent_getfd(bev);
    // server->bevMap.erase(fd);
    // LOG_TRACE << "buffer event map: " << (int)server->bevMap.size();
    TcpServerExt *servext = reinterpret_cast<TcpServerExt *>(channel->endpoint);
    --servext->total;
    return 0;
}

int onRead( Channel *channel)
{
    struct evbuffer *evbuf = bufferevent_get_input(channel->bufev);
    int len = evbuffer_get_length(evbuf);
    unsigned char *data = evbuffer_pullup(evbuf, len);
    int has_read = channel->decode((const char *)data, len); //consider put it in multiple threads
    evbuffer_drain(evbuf, has_read);

    return 0;
}

int onWrite( Channel *channel)
{
    LOG_INFO;
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
        else if(0 == strcmp(buffer, "count\n"))
        { 
            printf("recv_bytes: %ld\n", recv_bytes);
        }
    }

    // struct event *inputev = reinterpret_cast<struct event *>(data);
    // event_del(inputev);

    return;
}

int main(int argc, char **argv)
{

    TcpServerExt server;

    int features = event_base_get_features(server.base);
    LOG_INFO<<"support edge-trigger: "<<((features&EV_FEATURE_ET) ? "yes" : "no")
        <<", support one event: "<<((features&EV_FEATURE_ET) ? "yes" : "no")
        <<", support EV_CLOSED: "<<((features&EV_FEATURE_ET) ? "yes" : "no");

    Signal signal(server.base, &signal_cb, &server);

    server.onAccept = boost::bind(&onAccept, _1);
    server.onClose = boost::bind(&onClose, _1);
    server.onRead = boost::bind(&onRead, _1);
    server.onWrite = boost::bind(&onWrite, _1);

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
    TcpEndpoint::bufev_struct *bev = it->second;

    if (bev) {
        TcpEndpoint::Write(bev, msg, ::strlen(msg)); 
    }else{
        LOG_TRACE << "bufferevent is null";
    }
    
  }
}
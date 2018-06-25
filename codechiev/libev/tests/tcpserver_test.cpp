
#include <libev/TcpServer.hpp>
#include <libev/Signal.hpp>
#include <base/Logger.hpp>
#include <boost/bind.hpp>

using namespace codechiev::base;
using namespace codechiev::libev; 

static void signal_cb(evutil_socket_t, short, void *);

int onConnect(TcpServer *server, TcpServer::bufferevent_struct *bev)
{
    // server->bevMap[fd] = bev;
    // server->broadcast("foobar");
    server->write(bev, "welcome to visit");
    LOG_INFO << "";
    return 0;
}

int onClose(TcpServer *server, TcpServer::bufferevent_struct *bev)
{
    // evutil_socket_t fd = bufferevent_getfd(bev);
    // server->bevMap.erase(fd);
    // LOG_TRACE << "buffer event map: " << (int)server->bevMap.size();
    LOG_INFO << "";
    return 0;
}

int onRead(TcpServer *server, TcpServer::bufferevent_struct *bev, void *data, int len)
{
    std::string msg((char *)data, len);
    LOG_INFO << "read:" << len << "," << msg; 

    msg+=" echo";
    server->write(bev, msg.c_str());//within recursive locks
    return 0;
}

int onWrite(TcpServer *server, TcpServer::bufferevent_struct *bev)
{
    LOG_INFO << "";
    return 0;
}

int main(int argc, char **argv)
{
    TcpServer server(12345);
    Signal signal(server.base, &signal_cb, server.base);

    server.onConnect = boost::bind(&onConnect, &server, _1);
    server.onClose = boost::bind(&onClose, &server, _1);
    server.onRead = boost::bind(&onRead, &server, _1, _2, _3);
    server.onWrite = boost::bind(&onWrite, &server, _1);
    server.start();

    return 0;
}


static void
signal_cb(evutil_socket_t sig, short events, void *user_data)
{
    struct event_base *base = reinterpret_cast<struct event_base *>(user_data);
    struct timeval delay = {2, 0};

    printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

    event_base_loopexit(base, &delay);
}

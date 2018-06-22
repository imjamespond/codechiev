#include <stdio.h>

#include <base/Logger.hpp>
#include <libev/Timer.hpp>
#include <libev/TcpServer.hpp> 

using namespace codechiev::libev;

static void print(evutil_socket_t fd, short flags, void *data)
{
    LOG_INFO << "";
    TcpServer *server = reinterpret_cast<TcpServer *>(data);
    server->stop();
}

int main(int argc, const char * argv[]) {
    
    TcpServer server(12345l);
    Timer timer(server.base, &print, EV_PERSIST, &server);
    timer.timeout(2);
    server.start();

    return 0;
}

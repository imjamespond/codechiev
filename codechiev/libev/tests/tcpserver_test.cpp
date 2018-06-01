#include <stdio.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <libev/TcpServer.hpp>
#include <libev/SockAddress.hpp>

using namespace codechiev::libev;

int main(int argc, const char * argv[]) {
    
    TcpServer server(12345l);
    server.start();


    return 0;
}

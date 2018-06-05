#include <stdio.h>

#include <libev/TcpServer.hpp>
#include <libev/SockAddress.hpp>

using namespace codechiev::libev;

int main(int argc, const char * argv[]) {
    
    TcpServer server(12345l);
    server.start();


    return 0;
}

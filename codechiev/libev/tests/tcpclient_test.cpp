#include <stdio.h>

#include <libev/TcpClient.hpp>
#include <libev/SockAddress.hpp>

using namespace codechiev::libev;

int main(int argc, const char * argv[]) {

    const char hostname[] = "127.0.0.1:12345";
    TcpClient client(hostname);
    client.connect();

    return 0;
}

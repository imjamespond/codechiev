#include <stdio.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <libev/TcpClient.hpp>
#include <libev/SockAddress.hpp>

using namespace codechiev::libev;

int main(int argc, const char * argv[]) {

    const char hostname[] = "127.0.0.1:12345";
    TcpClient client(hostname);
    client.connect();

    return 0;
}

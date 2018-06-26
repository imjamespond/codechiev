#include <stdio.h>

#include <libev/TcpClient.hpp>
#include <libev/SockAddress.hpp>

using namespace codechiev::libev;

int main(int argc, const char * argv[]) {

    const char hostname[] = "127.0.0.1:12345";
    TcpClient client(hostname);

    // client.onConnect = boost::bind(&onConnect, &client, _1);
    // client.onClose = boost::bind(&onClose, &client, _1);
    // client.onRead = boost::bind(&onRead, &client, _1, _2, _3);
    // client.onWrite = boost::bind(&onWrite, &client, _1);
    // client.start(EVLOOP_NONBLOCK);
    // client.connect();
    for(int i=0; i<10; i++)
    {
        client.connect();
    }
    client.start();

    return 0;
}

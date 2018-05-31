#include <stdio.h> 

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <libev/TcpServer.hpp>

using namespace codechiev::libev;

int main(int argc, const char * argv[]) {
    struct event_base *base;
    struct evconnlistener *listener;

    base = event_base_new();
    if (!base)
    {
        fprintf(stderr, "Could not initialize libevent!\n");
        return 1;
    }

    return 0;
}

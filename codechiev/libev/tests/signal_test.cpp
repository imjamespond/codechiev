#include <stdio.h>

#include <base/Logger.hpp>
#include <libev/Signal.hpp> 

using namespace codechiev::libev;

int called = 0;

static void
signal_cb(evutil_socket_t fd, short event, void *arg)
{
    struct event *signal = reinterpret_cast<struct event *>(arg);

    printf("signal_cb: got signal %d\n", event_get_signal(signal));

    if (called >= 2)
        event_del(signal);

    called++;
}

int main(int argc, const char * argv[]) {

    Signal signal(&signal_cb, NULL);
    signal.start();

    return 0;
}

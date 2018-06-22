#include <stdio.h>

#include <base/Logger.hpp>
#include <libev/Timer.hpp> 

using namespace codechiev::libev;

static void print(evutil_socket_t fd, short flags, void *data)
{
    LOG_INFO << "";

    static int count = 3;
    struct event_base *base = reinterpret_cast<struct event_base *>(data);
    struct timeval delay = {2, 0};
    base && --count == 0 && event_base_loopexit(base, &delay);
}

int main(int argc, const char * argv[]) {

    Timer timer(&print, EV_PERSIST, NULL);
    timer.timeout(2);
    timer.start();

    return 0;
}

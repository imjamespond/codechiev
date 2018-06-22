#include <stdio.h>

#include <base/Logger.hpp>
#include <libev/Timer.hpp> 

using namespace codechiev::libev;

static void print(evutil_socket_t fd, short flags, void *data)
{
    LOG_INFO << "";

    static int count = 3;
    Timer *timer = reinterpret_cast<Timer *>(data);
    struct timeval delay = {2, 0};
    timer && --count == 0 && timer->stop();
}

int main(int argc, const char * argv[]) {

    Timer timer(&print, EV_PERSIST, NULL);
    timer.timeout(2);
    timer.start();

    return 0;
}

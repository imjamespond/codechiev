#include <stdio.h>

#include <base/Logger.hpp>
#include <libev/Timer.hpp> 

using namespace codechiev::libev;

static void print(evutil_socket_t fd, short flags, void *data)
{
    LOG_INFO<<"";
}

int main(int argc, const char * argv[]) {

    Timer timer(&print);
    timer.timeout();

    return 0;
}

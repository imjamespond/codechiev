#include <stdio.h>

#include <base/Logger.hpp>
#include <libev/Timer.hpp> 

using namespace codechiev::libev;

void print(Timer *timer)
{
    LOG_INFO;

    static int count = 3;
    // struct timeval delay = {2, 0};
    timer && --count == 0 && timer->stop();
}

int main(int argc, const char * argv[]) 
{
    struct event_base *base = event_base_new();
    Timer timer(base, EV_PERSIST); 
    timer.timeout(boost::bind(print, &timer),1);
    event_base_dispatch(base);

    return 0;
}

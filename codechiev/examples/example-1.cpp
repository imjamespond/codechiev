// A simple blocking HTTP client
// http://www.wangafu.net/~nickm/libevent-book/01_intro.html

#include <event2/event.h>
#include <event2/thread.h>

#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int c, char **v)
{
    event_enable_debug_mode();
    return 0;
}
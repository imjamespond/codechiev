#ifndef Timer_h
#define Timer_h

#include <string.h> //memset

#include <event2/event.h>
#include <event2/event_struct.h>

#include <time.h>
#include <sys/time.h>

namespace codechiev
{
namespace libev
{

struct Timer
{
    explicit Timer(event_callback_fn);
    ~Timer();

    void timeout();

    struct event _timeout;
    struct timeval tv;
    struct event_base *base;
    int flags;
};

} // namespace libev
} // namespace codechiev

#endif /* Timer_h */

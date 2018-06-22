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
    typedef struct event event_struct;
    typedef struct event_base event_base_struct;

//  typedef void (*event_callback_fn)(evutil_socket_t, short, void *)
    explicit Timer(event_base_struct*, event_callback_fn, int, void*);
    explicit Timer(event_callback_fn, int, void*);
    ~Timer();

    void timeout(int);
    void start();
    int stop();

    event_struct _timeout;
    event_base_struct *base;

    struct timeval tv;
};

} // namespace libev
} // namespace codechiev

#endif /* Timer_h */

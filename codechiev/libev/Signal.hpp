#ifndef Signal_h
#define Signal_h

#include <event2/event-config.h>
#include <event2/event.h>

#include <signal.h>

namespace codechiev
{
namespace libev
{

struct Signal
{
    typedef struct event event_struct;
    typedef struct event_base event_base_struct;

//  typedef void (*event_callback_fn)(evutil_socket_t, short, void *)
    explicit Signal(event_base_struct *, event_callback_fn, void *);
    explicit Signal(event_callback_fn, void *);
    ~Signal();

    void start();
    int stop();

    event_base_struct *base;
    event_struct *event;
};

} // namespace libev
} // namespace codechiev

#endif /* Signal_h */

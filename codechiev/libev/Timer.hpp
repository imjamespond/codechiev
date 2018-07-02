#ifndef Timer_h
#define Timer_h

#include <string.h> //memset

#include <event2/event.h>
#include <event2/event_struct.h>

#include <time.h>
#include <sys/time.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace codechiev
{
namespace libev
{

struct Timer
{
    typedef struct event event_struct;
    typedef struct event_base event_base_struct;

    typedef boost::function<void()> timeout_callback_fn;
    explicit Timer(event_base_struct *, int = EV_READ | EV_ET);
    // explicit Timer(event_base_struct *, event_callback_fn, int = EV_READ, void * = NULL);
    // explicit Timer(event_callback_fn, int, void*);
    // ~Timer();

    void timeout(int = 0);
    void timeout(const timeout_callback_fn&, int = 0);
    void start();
    int stop();

    // event_struct timeout_;
    event_struct *timeout_;
    // event_base_struct *base_;

    struct timeval timeval_;

    timeout_callback_fn callback;
};

} // namespace libev
} // namespace codechiev

#endif /* Timer_h */

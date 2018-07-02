#include "Timer.hpp"

using namespace codechiev::libev;

//desired events to monitor: bitfield of EV_READ, EV_WRITE, EV_SIGNAL, EV_PERSIST, EV_ET
//The EV_PERSIST flag can also be passed in the events argument: it makes event_add() persistent until event_del() is called.
//The EV_ET flag is compatible with EV_READ and EV_WRITE, and supported only by certain backends. It tells Libevent to use edge-triggered events.

void timeout_callback(evutil_socket_t fd, short flags, void *data);

Timer::Timer(event_base_struct *base, int flags)
    :timeout_(NULL), callback(0)
{ 
  /* Initalize one event */
  timeout_ = event_new(base, -1, flags, timeout_callback, this);
}

// Timer::Timer(event_base_struct *base, event_callback_fn timeout_cb, int flags, void *data)
//     : base_(NULL), timeout_(NULL)
// {
//   /* Initalize one event */
//   event_assign(&timeout_, base, -1, flags, timeout_cb, data ? data : this);
// }

// Timer::Timer(event_callback_fn timeout_cb, int flags, void *data)
//     : base_(NULL), timeout_(NULL)
// {
//   /* Initalize the event library */
//   base_ = event_base_new();

//   /* Initalize one event */
//   event_assign(&timeout_, base_, -1, flags, timeout_cb, data ? data : this);
// }

// Timer::~Timer()
// {
//   if (base_)
//     event_base_free(base_);
// }

void 
Timer::timeout(int sec){
  evutil_timerclear(&timeval_);
  timeval_.tv_sec = sec;
  event_add(timeout_, &timeval_);
}

void 
Timer::timeout(const timeout_callback_fn &cb, int sec)
{
  callback = cb;
  timeout(sec);
}

// void Timer::start()
// {
//   base_ && event_base_dispatch(base_);
// }

int Timer::stop()
{
  timeout_ && event_del(timeout_);

  // if(base_)
  //   return event_base_loopexit(base_, NULL);
  // else
  
  return 0;
}

void timeout_callback(evutil_socket_t fd, short flags, void *data)
{
  Timer *timer = reinterpret_cast<Timer *>(data);
  if (timer && timer->callback)
  {
    timer->callback();
  }
}
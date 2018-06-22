#include "Timer.hpp"

using namespace codechiev::libev;

Timer::Timer(event_base_struct *base, event_callback_fn timeout_cb, int flags, void* data):
base(NULL)
{
  /* Initalize one event */
  event_assign(&_timeout, base, -1, flags, timeout_cb, data ? data : this);
}

Timer::Timer(event_callback_fn timeout_cb, int flags, void* data)
{
  /* Initalize the event library */
  base = event_base_new();

  /* Initalize one event */
  event_assign(&_timeout, base, -1, flags, timeout_cb, data ? data : this);
}

Timer::~Timer()
{
  if (base)
    event_base_free(base);
}

void 
Timer::timeout(int sec){
  evutil_timerclear(&tv);
  tv.tv_sec = sec;
  event_add(&_timeout, &tv);
}

void Timer::start()
{
  base && event_base_dispatch(base);
}

int Timer::stop()
{
  return base && event_base_loopexit(base, NULL);
}
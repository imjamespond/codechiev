#include "Timer.hpp"

using namespace codechiev::libev;

Timer::Timer(event_base_struct *base, event_callback_fn timeout_cb, int flags, void* data):
base(base)
{
  /* Initalize one event */
  event_assign(&_timeout, base, -1, flags, timeout_cb, (void *)(data || this));
}

Timer::Timer(event_callback_fn timeout_cb, int flags, void* data)
{
  /* Initalize the event library */
  base = event_base_new();

  /* Initalize one event */
  event_assign(&_timeout, base, -1, flags, timeout_cb, (void *)(data || base));
}

Timer::~Timer()
{
}

void 
Timer::timeout(int sec){
  evutil_timerclear(&tv);
  tv.tv_sec = sec;
  event_add(&_timeout, &tv);
}

void Timer::start()
{
  event_base_dispatch(base);
}

int Timer::stop()
{
  return event_base_loopbreak(base);
}
#include "Timer.hpp"

using namespace codechiev::libev;

Timer::Timer(event_callback_fn timeout_cb)
{
  /* Initalize the event library */
  base = event_base_new();

  /* Initalize one event */
  event_assign(&_timeout, base, -1, flags, timeout_cb, (void *)&_timeout);
}

Timer::~Timer()
{
}

void 
Timer::timeout(){
  evutil_timerclear(&tv);
  tv.tv_sec = 2;
  event_add(&_timeout, &tv);
}
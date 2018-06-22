#include "Signal.hpp"

using namespace codechiev::libev;

Signal::Signal(event_base_struct *base, event_callback_fn cb, void* data):
base(base)
{
  /* Initalize one event */
  event = evsignal_new(base, SIGINT, cb, data ? data : event_self_cbarg());

  event_add(event, NULL);
}

Signal::Signal(event_callback_fn cb, void *data)
{
  /* Initalize the event library */
  base = event_base_new();

  /* Initalize one event */
  event = evsignal_new(base, SIGINT, cb, data ? data : event_self_cbarg());

  event_add(event, NULL);
}

Signal::~Signal()
{
  event_free(event);
}

void Signal::start()
{
  event_base_dispatch(base);
}

int Signal::stop()
{
  return event_base_loopbreak(base);
}
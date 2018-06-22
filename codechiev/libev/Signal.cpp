#include "Signal.hpp"

using namespace codechiev::libev;

Signal::Signal(event_base_struct *base, event_callback_fn cb):
base(base)
{

}

Signal::Signal(event_callback_fn cb)
{
  /* Initalize the event library */
  base = event_base_new();

  /* Initalize one event */
  signal = evsignal_new(base, SIGINT, cb, event_self_cbarg());

  event_add(signal, NULL);
}

Signal::~Signal()
{
  event_free(signal);
  event_base_free(base);
}

void Signal::start()
{
  event_base_dispatch(base);
}

int Signal::stop()
{
  return event_base_loopbreak(base);
}
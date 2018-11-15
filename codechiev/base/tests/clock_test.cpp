#include <stdio.h>
#include <stdint.h>
#include <base/Time.hpp>
#include <base/Logger.hpp>

using namespace codechiev::base;

int main(int argc, const char *argv[])
{
  LOG_INFO << Time::Now().getMillis();
  LOG_INFO << Time::NowTm().getMillis();
  LOG_INFO << Time::NowClock().getMillis();

  Time now = Time::Now().getMillis();
  LOG_INFO << "\n"
           << MILLIS_TO_SECS(now.getMillis() + 1000l)
           << "\n"
           << MILLIS_TO_SECS((now.getMillis() + 1000l));
  return 0;
}

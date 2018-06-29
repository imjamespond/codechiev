#ifndef Time_hpp
#define Time_hpp

#include <stdint.h>
#include <string>

namespace codechiev
{
namespace base
{
/*
        static const int64_t MILLIS_SEC = 1000;
        static const int64_t MICROS_SEC = MILLIS_SEC * 1000;
        static const int64_t NANOS_SEC = MICROS_SEC * 1000;
        static const float SEC_OF_MILLI = ;
        */
#define MILLIS_TO_NANOS(millis) millis % 1000 * 1000000
#define MILLIS_TO_SECS(millis) millis * .001
#define SECS_TO_MILLIS(secs) static_cast<int64_t>(secs) * 1000
#define MICROS_TO_MILLIS(micros) micros * .001
#define NANOS_TO_MILLIS(nanos) nanos * .000001

class Time
{
  public:
    Time(int64_t t = 0);

    static Time Now();
    static Time NowTm();
    static Time GetClockTime();

    static void
        SleepMillis(int64_t);

    static std::string
    GetSimpleString();

    inline int64_t
    getMillis() { return timeMillis_; }

    void
    operator=(int64_t);

    int64_t
    operator-(Time &);

  private:
    int64_t timeMillis_;
};
} // namespace base
} // namespace codechiev

#endif /* Time_hpp */

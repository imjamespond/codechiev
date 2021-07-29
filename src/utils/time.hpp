#ifndef Time_hpp
#define Time_hpp

#include <sys/time.h>
#include <stdint.h>
#include <string>
#include <ctime>

namespace learn_cpp
{ 

#define MILLIS_TO_NANOS(millis) millis % 1000 * 1000000
#define MILLIS_TO_SECS(millis) static_cast<int64_t>(millis * .001)
#define SECS_TO_MILLIS(secs) static_cast<int64_t>(secs) * 1000
#define MICROS_TO_MILLIS(micros) static_cast<int64_t>(micros * .001)
#define NANOS_TO_MILLIS(nanos) static_cast<int64_t>(nanos * .000001)

class Time
{
public: 
  static inline Time Now()
  {
    struct timeval tv;
    ::gettimeofday(&tv, NULL);
    return Time(SECS_TO_MILLIS(tv.tv_sec) + MICROS_TO_MILLIS(tv.tv_usec));
  }
  static inline Time NowTm()
  {
    time_t rawtime;
    // struct tm * ptm;
    ::time(&rawtime);
    // ptm = gmtime(&rawtime);
    return Time(SECS_TO_MILLIS(rawtime)); //less acurate
  }
  static inline Time NowClock()
  {
    struct timespec now;

    if (::clock_gettime(CLOCK_REALTIME, &now) == -1)
        perror("clock_gettime()");

    return Time(SECS_TO_MILLIS(now.tv_sec) + NANOS_TO_MILLIS(now.tv_nsec));
  }
  static inline std::string LocalTime()
  {
      std::time_t result = std::time(NULL);
      return std::asctime(std::localtime(&result));
  }

  Time(int64_t t = 0){ millis = t;}

  inline int64_t getMillis() { return millis; }

  private:
    int64_t millis;
}; 
} // namespace  

#endif /* Time_hpp */
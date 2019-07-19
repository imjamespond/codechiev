#include "Time.hpp"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#ifdef __MINGW32__
#include <windows.h>
#endif // __MINGW32__

using namespace codechiev::base;

Time::Time(int64_t t) : timeMillis_(t) {}

Time Time::Now()
{
    struct timeval tv;
    ::gettimeofday(&tv, NULL);
    return Time(SECS_TO_MILLIS(tv.tv_sec) + MICROS_TO_MILLIS(tv.tv_usec));
}

Time Time::NowTm()
{
    time_t rawtime;
    // struct tm * ptm;
    ::time(&rawtime);
    // ptm = gmtime(&rawtime);
    return Time(SECS_TO_MILLIS(rawtime)); //less acurate
}

Time Time::NowClock()
{
    struct timespec now;

    if (::clock_gettime(CLOCK_REALTIME, &now) == -1)
        perror("clock_gettime()");

    return Time(SECS_TO_MILLIS(now.tv_sec) + NANOS_TO_MILLIS(now.tv_nsec));
}

void Time::SleepMillis(int64_t millis)
{
    struct timespec tm;
    tm.tv_sec = MILLIS_TO_SECS(millis);
    tm.tv_nsec = MILLIS_TO_NANOS(millis);
    /*
    int nanosleep(const struct timespec *req, struct timespec *rem);
        If the call is interrupted by a signal handler, nanosleep() returns
    -1, sets errno to EINTR, and writes the remaining time into the
    structure pointed to by rem unless rem is NULL.  The value of *rem
    can then be used to call nanosleep() again and complete the specified
    pause (but see NOTES).*/

#ifdef __MINGW32__
    Sleep(millis);
#else
    ::nanosleep(&tm, NULL);
#endif // __MINGW32__
}

void Time::operator=(int64_t t)
{
    this->timeMillis_ = t;
}

int64_t
Time::operator-(Time &t)
{
    return this->timeMillis_ - t.getMillis();
}

bool Time::operator>(Time &t)
{
    return this->timeMillis_ > t.getMillis();
}

bool Time::operator<(Time &t)
{
    return this->timeMillis_ < t.getMillis();
}

bool Time::operator==(Time &t)
{
    return this->timeMillis_ == t.getMillis();
}

#include <ctime>
std::string
Time::GetSimpleString()
{
    std::time_t result = std::time(NULL);
    return std::asctime(std::localtime(&result));
}

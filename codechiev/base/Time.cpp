//
//  Time.cpp
//  codechiev
//
//  Created by metasoft on 16/7/19.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "Time.hpp"
#include <time.h>
#include <sys/time.h>

using namespace codechiev::base;

Time::Time(int64_t t):timeMillis_(t){}

Time Time::now() {
    struct timeval tv;
    ::gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return Time(seconds * MILLIS_SEC + tv.tv_usec * .001);
}

Time Time::nowTm() {
    time_t rawtime;
    struct tm * ptm;
    ::time(&rawtime);
    ptm = gmtime(&rawtime);
    return Time(static_cast<int64_t> (rawtime) * MILLIS_SEC);//less acurate
}

void
Time::SleepMillis(int64_t millis)
{
    struct timespec tm;
    tm.tv_sec=millis*SEC_OF_MILLI;
    tm.tv_nsec=(millis-tm.tv_sec*MILLIS_SEC)*1000000;
    /*
    int nanosleep(const struct timespec *req, struct timespec *rem);
        If the call is interrupted by a signal handler, nanosleep() returns
    -1, sets errno to EINTR, and writes the remaining time into the
    structure pointed to by rem unless rem is NULL.  The value of *rem
    can then be used to call nanosleep() again and complete the specified
    pause (but see NOTES).*/
    ::nanosleep(&tm, nullptr);
}

void
Time::operator=(int64_t t)
{
    this->timeMillis_ = t;
}

int64_t
Time::operator-(Time & t)
{
    return this->timeMillis_-t.getMillis();
}
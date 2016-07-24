//
//  Timer.cpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "Timer.hpp"
#include "Channel.hpp"
#include <base/Logger.hpp>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <sys/timerfd.h>

using namespace codechiev::net;

#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)

/*These system calls create and operate on a timer that delivers timer
       expiration notifications via a file descriptor.They provide an
       alternative to the use of setitimer(2) or timer_create(2), with the
       advantage that the file descriptor may be monitored by select(2),
       poll(2), and epoll(7).

       The use of these three system calls is analogous to the use of
       timer_create(2), timer_settime(2), and timer_gettime(2).  (There is
       no analog of timer_getoverrun(2), since that functionality is
       provided by read(2), as described below.)

   timerfd_create()
       timerfd_create() creates a new timer object, and returns a file
       descriptor that refers to that timer.  The clockid argument specifies
       the clock that is used to mark the progress of the timer, and must be
       either *****CLOCK_REALTIME or CLOCK_MONOTONIC单调的*****.  CLOCK_REALTIME is a
       settable system-wide clock.  *****CLOCK_MONOTONIC is a nonsettable***** clock
       that is not affected by discontinuous changes in the system clock
       (e.g., manual changes to system time).  The current value of each of
       these clocks can be retrieved using clock_gettime(2).

       Starting with Linux 2.6.27, the following values may be bitwise ORed
       in flags to change the behavior of timerfd_create():

       TFD_NONBLOCK  Set the O_NONBLOCK file status flag on the new open
                     file description.  Using this flag saves extra calls to
                     fcntl(2) to achieve the same result.

       TFD_CLOEXEC   Set the close-on-exec (FD_CLOEXEC) flag on the new file
                     descriptor.  See the description of the O_CLOEXEC flag
                     in open(2) for reasons why this may be useful.

       In Linux versions up to and including 2.6.26, flags must be specified
       as zero.
       */
Timer::Timer():
    channel_(timerfd_create(CLOCK_REALTIME, TFD_CLOEXEC))
{
    assert(channel_.getFd()>-1);
}

void
Timer::setTime()
{
    /*   timerfd_settime()
       The flags argument is *****either 0, to start a *****relative timer
       (*****new_value.it_value specifies a time *****relative to the current value of
       the clock specified by clockid), *****or TFD_TIMER_ABSTIME, to start an
       *****absolute timer (new_value.it_value specifies an absolute time for the
       clock specified by clockid; that is, the timer will expire when the
       value of that clock reaches the value specified in
       new_value.it_value).

       If the *****old_value argument is not NULL, then the *****itimerspec structure
       that it points to is used to *****return the setting of the timer***** that was
       current at the time of the call; see the description of
       timerfd_gettime() following.
 Set next expiration time of interval timer source UFD to UTMR.  If
   FLAGS has the TFD_TIMER_ABSTIME flag set the timeout value is
   absolute.  Optionally return the old expiration time in OTMR.
extern int timerfd_settime (int __ufd, int __flags,
			    __const struct itimerspec *__utmr,
			    struct itimerspec *__otmr) __THROW;*/

        int max_exp, fd;



        struct itimerspec oldtime;


}

void
Timer::after(int64_t secs)
{
    struct itimerspec new_value;
    struct itimerspec old_value;
    struct timespec now;
    if (clock_gettime(CLOCK_REALTIME, &now) == -1)
        handle_error("clock_gettime");
    new_value.it_value.tv_sec = now.tv_sec + secs;
    new_value.it_value.tv_nsec = now.tv_nsec;

    new_value.it_interval.tv_sec = 2000;
    new_value.it_interval.tv_nsec = 0;

    ::timerfd_settime(channel_.getFd(), TFD_TIMER_ABSTIME, &new_value, &old_value);

    uint64_t exp(0);
    while(1)
    {
        int len = ::read(channel_.getFd(), &exp, sizeof(uint64_t));
        if(len==sizeof(uint64_t))
            LOG_DEBUG<<"time's up";
    }
}

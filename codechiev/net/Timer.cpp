//
//  Timer.cpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "Timer.hpp"
#include "Channel.hpp"
#include <assert.h>
#include <sys/timerfd.h>

using namespace codechiev::net;

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
    channel_(timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK|TFD_CLOEXEC))
{
    assert(channel_.getFd()>-1);
}

void
Timer::setTime()
{

}

void
Timer::after()
{

}

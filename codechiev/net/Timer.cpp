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
#include <base/Time.hpp>
#include <boost/bind.hpp>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <sys/timerfd.h>
#include <sys/socket.h>

using namespace codechiev::net;
using namespace codechiev::base;

#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)

Timer::Timer():
channel_(::timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK|TFD_CLOEXEC))
{
    if (channel_.getFd() == -1) {
        perror("timerfd_create");
        exit(EXIT_FAILURE);
    }
}

void
Timer::after(int64_t millis)
{
    every(0, millis);
}

void
Timer::every(int64_t millis, int64_t delay)
{
    struct itimerspec new_value;
    struct itimerspec old_value;
    struct timespec now;
    if (clock_gettime(CLOCK_REALTIME, &now) == -1)
        handle_error("clock_gettime");
    new_value.it_value.tv_sec = now.tv_sec + MILLIS_TO_SECS(delay);
    new_value.it_value.tv_nsec = now.tv_nsec + MILLIS_TO_NANOS(delay);
    
    new_value.it_interval.tv_sec = MILLIS_TO_SECS(millis);
    new_value.it_interval.tv_nsec = MILLIS_TO_NANOS(millis);
    
    ::timerfd_settime(channel_.getFd(), TFD_TIMER_ABSTIME, &new_value, &old_value);
}

Scheduler::Scheduler()
{}

void
Scheduler::schedule()
{
    loop_.loop(boost::bind(&Scheduler::pollEvent, this, _1));
}
void
Scheduler::scheduleTimer(Timer &timer)
{
    timer.getChannel()->setEvent(EPOLLIN);
    loop_.getPoll().addChannel(timer.getChannel());
}
void
Scheduler::pollEvent(const channel_vec& vec)
{
    for(channel_vec::const_iterator it=vec.begin();
        it!=vec.end();
        it++)
    {
        Channel *channel = *it;
        typedef uint64_t data_t;
        data_t data(0);
        ssize_t len = ::read(channel->getFd(), &data, sizeof(data_t));        LOG_DEBUG<<"read:"<<len<<", fd:"<<channel->getFd()<<", errno:"<<errno;
        if(len==sizeof(data_t))
            LOG_DEBUG<<"time's up";
    }
}

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
#include <base/Mutex.hpp>
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

Timer::Timer():next(0),
channel_(::timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK|TFD_CLOEXEC))
{
    if (channel_.getFd() == -1)
    {
        perror("timerfd_create");
        exit(EXIT_FAILURE);
    }
}
Timer::~Timer()
{
    channel_.close();
    LOG_TRACE<<"";
}

void
Timer::after(int64_t millis)
{
    every(0, millis);
}
void
Timer::after(int64_t millis, const timer_cb_t &cb)
{
    cb_=cb;
    after(millis);
}
void
Timer::every(int64_t millis, int64_t delay, const timer_cb_t &cb)
{
    cb_=cb;
    every(millis, delay);
}
void
Timer::every(int64_t millis, int64_t delay)
{
    struct itimerspec new_value;
    struct itimerspec old_value;
    struct timespec now;
    if (::clock_gettime(CLOCK_REALTIME, &now) == -1)
        handle_error("clock_gettime");
    
    new_value.it_value.tv_sec = now.tv_sec + MILLIS_TO_SECS(delay);
    new_value.it_value.tv_nsec = now.tv_nsec + MILLIS_TO_NANOS(delay);

    new_value.it_interval.tv_sec = MILLIS_TO_SECS(millis);
    new_value.it_interval.tv_nsec = MILLIS_TO_NANOS(millis);

    ::timerfd_settime(channel_.getFd(), TFD_TIMER_ABSTIME, &new_value, &old_value);
}
void
Timer::expireAt(int64_t millis)
{
    struct itimerspec new_value;
    struct itimerspec old_value;
    
    ::memset(&new_value, 0, sizeof(struct itimerspec));
             
    new_value.it_value.tv_sec = MILLIS_TO_SECS(millis);
    new_value.it_value.tv_nsec = MILLIS_TO_NANOS(millis);
    
    ::timerfd_settime(channel_.getFd(), TFD_TIMER_ABSTIME, &new_value, &old_value);
    
}

#define TIMER_ACCURACY 30
TimerQueue::TimerQueue():timer_(new Timer)
{}
void
TimerQueue::commence()
{
    timer_->setCallback(boost::bind(&TimerQueue::expire, this));
    schedule_.addTimer(timer_);
    schedule_.schedule();
}
void
TimerQueue::addTask(int64_t expired, const timer_cb & task)
{
    assert(task);
    task_map::iterator it=tasks_.begin();
    if(it != tasks_.end() && it->first-expired<TIMER_ACCURACY)
    {
        MutexGuard lock(&mutex_);
        tasks_.insert(task_pair(expired, task));
    }
    else if( expired-Time::Now().getMillis()<TIMER_ACCURACY )
    {
        task();
    }else
    {
        MutexGuard lock(&mutex_);
        tasks_.insert(task_pair(expired, task));
        timer_->expireAt(expired);
    }
}
void
TimerQueue::expire()
{
    MutexGuard lock(&mutex_);
    
    task_vec tasks;
    for(task_map::iterator it=tasks_.begin();
        it!=tasks_.end(); )
    {
        if(it->first - Time::Now().getMillis()<TIMER_ACCURACY)
        {
            tasks.push_back(it->second);//might lock recursively in callback
            tasks_.erase(it++);
        }else
        {
            timer_->expireAt(it->first);
            break;
        }
    }
    
    for(task_vec::const_iterator it=tasks.begin();
        it!=tasks.end();
        it++)
    {
        (it->second)();
    }
}

Scheduler::Scheduler()
{}

void
Scheduler::schedule()
{
    loop_.loop(boost::bind(&Scheduler::pollEvent, this, _1));
}
void
Scheduler::addTimer(const timer_ptr &timer)
{
    assert(timer);
    timers_[timer->getChannel()->getFd()] = timer;
    timer->getChannel()->setEvent(EPOLLIN);
    loop_.getPoll().addChannel(timer->getChannel());
}
void
Scheduler::setTimer(const timer_ptr &timer)
{
    assert(timer);
    timer->getChannel()->setEvent(EPOLLIN);
    loop_.getPoll().setChannel(timer->getChannel());
}
void
Scheduler::removeTimer(int fd)
{
    timer_map::const_iterator it = timers_.find( fd );
    if(it!=timers_.end())
    {
        loop_.getPoll().delChannel(it->second->getChannel());
        timers_.erase(fd);//probitally destruct
    }
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
        ssize_t len = ::read(channel->getFd(), &data, sizeof(data_t));
        if(len==sizeof(data_t))
        {
            timer_map::const_iterator it = timers_.find( channel->getFd());
            if(it!=timers_.end())
            {
                it->second->callback();
            }
        }
    }
}

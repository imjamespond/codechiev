//
//  main.cpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <stdio.h>
#include <boost/bind.hpp>
#include <errno.h>
#include <exception>
#include <base/Time.hpp>
#include <base/Logger.hpp>
#include <base/Thread.hpp>
#include <net/Timer.hpp>
#include <net/EPoll.hpp>

using namespace codechiev;

net::TimerQueue timerq;
//net::Scheduler sc;
void print(int s)
{
    LOG_DEBUG<<"time's up, sec:"<<s;
}
void quit(base::Thread* t)
{
    LOG_DEBUG<<"timer quit";
    t->cancel();
}
void countdown(int c)
{
    LOG_DEBUG<<"countdown left:"<<c;
    if(--c)
    {
        base::Time now=base::Time::Now();
        timerq.addTask(now.getMillis()+100l, boost::bind(&countdown, c));
    }
}
int main(int argc, const char * argv[]) {
    /*base::Thread thread("Scheduler", boost::bind(&net::Scheduler::schedule, &sc));
    thread.start();

    {
    net::timer_ptr t1(new net::Timer),t2(new net::Timer),t3(new net::Timer);
    t1->after(8000l, boost::bind(&print, t1->getChannel()->getFd()));
    t2->every(1000l, 5000l, boost::bind(&cease, t2->getChannel()->getFd()));
    t3->after(2000l, boost::bind(&print, t3->getChannel()->getFd()));
    sc.addTimer(t1);
    sc.addTimer(t2);
    sc.addTimer(t3);
    }*/
    
    base::Thread thread("TimerQueue", boost::bind(&net::TimerQueue::commence, &timerq));
    thread.start();
    
    LOG_DEBUG<<"timer test";
    base::Time now=base::Time::Now();
    timerq.addTask(now.getMillis()+9000l, boost::bind(&quit, &thread));
    timerq.addTask(now.getMillis()+3000l, boost::bind(&print, 3));
    timerq.addTask(now.getMillis()+6000l, boost::bind(&countdown, 9));
    timerq.addTask(now.getMillis()+30l, boost::bind(&print, 0));
    
    thread.join();
    /*test blocking fd
    uint64_t exp(0);
    while(1)
    {
        ssize_t len = ::read(timer.getChannel().getFd(), &exp, sizeof(uint64_t));
        //LOG_DEBUG<<"read";
        if(len==sizeof(uint64_t))
            LOG_DEBUG<<"time's up";
    }*/
    /*
    net::Timer timer;
    net::EPoll epoll;
    epoll.addChannel(&timer.getChannel());

    for(int i=0; i<3; i++)
    {
        timer.after(3);
        net::channel_vec vec;
        epoll.poll(vec);
        for(net::channel_vec::iterator it=vec.begin();
            it!=vec.end();
            it++)
        {
            net::Channel *channel = *it;
            typedef uint64_t data_t;
            data_t data(0);
            ssize_t len = ::read(channel->getFd(), &data, sizeof(data_t));//test level-trigger
            /*EINVAL fd was created via a call to timerfd_create(2) and the wrong
              size buffer was given to read(); see timerfd_create(2) for
              further information.* /
            LOG_DEBUG<<"read:"<<len<<", fd:"<<channel->getFd()<<", errno:"<<errno;
            if(len==sizeof(data_t))
                LOG_DEBUG<<"time's up";
        }
    }*/
    return 0;
}

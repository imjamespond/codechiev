//
//  main.cpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <errno.h>

#include "base/Thread.hpp"
#include "base/Condition.hpp"
#include "base/Mutex.hpp"
#include "base/Time.hpp"
#include "base/Random.hpp"

using namespace codechiev::base;

Condition cond;
Mutex cond_mutex;
Mutex user_mutex;
int count(0);
void print()
{
    while(1)
    {
        {
            MutexGuard lock(&cond_mutex);
            while(count==0)
            {
                cond.wait(cond_mutex);//release mutex and block here
            }
            printf("thread wake: %s %d, and should do some job assignment here:%d\n",
                   Thread::ThreadName().c_str(), Thread::GetTid(), count);
            if(--count)
                cond.notify();
        }
        
        {
            //MutexGuard lock(&user_mutex);
            int64_t millis = random(500, 2000);
            Time::SleepMillis(millis);
            printf("finish job here: %s %d, sleep for: %lld\n",
                   Thread::ThreadName().c_str(), Thread::GetTid(), millis);
        }

    }
}

void wake(int ch)
{
    MutexGuard lock(&cond_mutex);
    
    if(ch=='a')
    {
        count+=15;
        cond.notifyall();
    }
    if(ch=='o')
    {
        count+=5;
        cond.notify();
    }
}

int main(int argc, const char * argv[]) {
    
    Thread* threads[8];
    
    for(int i=0;i<(sizeof(threads)/sizeof(Thread*));i++)
    {
        std::string name("thread-");
        name+=boost::lexical_cast<std::string>(i);
        auto t = new Thread(name, boost::bind(&print));
        threads[i] = t;
        t->start();
    }
    
    
    int ch;
    do
    {
        ch=getchar();
        wake(ch);
        
    }while(ch!='.');
    
    
    for(int i=0;i<(sizeof(threads)/sizeof(Thread*));i++)
    {
        threads[i]->join();
    }
    return 0;
}

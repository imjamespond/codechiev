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

#include "base/BlockingQueue.hpp"
#include "base/Random.hpp"
#include "base/Time.hpp"
#include "base/Logger.hpp"

using namespace codechiev::base;

int count(0);
void print()
{
    if(count)
    {
        count=0;
        int64_t millis = random(1, 10);
        Time::SleepMillis(millis);
        if(count)
        {
            LOG_TRACE<<"sleep:"<<millis<< " thread:"<<Thread::ThreadName()<< " tid:"<<Thread::GetTid();
        }
    }
    else
    {
        count++;
    }
    
    //printf("current thread: %s - %d, %d\n",Thread::ThreadName().c_str(), Thread::GetTid(), errno);
}

int main(int argc, const char * argv[]) {
    
    ::setLoggerDetail(false);
    
    BlockingQueue<10> queue;
    queue.commence();
    
    for(int i=0; i<999; i++)
    {
        queue.addJob(boost::bind(&print));
    }
    
    //Time::SleepMillis(5000l);
    queue.addJob(boost::bind(&BlockingQueue<10>::stop, &queue));
    
    return 0;
}

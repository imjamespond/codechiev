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
#include <boost/assert.hpp>
#include <vector>
#include <base/Logger.hpp>
#include <base/Thread.hpp>
#include <base/Time.hpp> 
#include <base/Condition.hpp>
#include <base/Mutex.hpp>

using namespace codechiev::base;

#define ThreadNum 10

CountLatch ThreadLatch(1);
CountLatch MainLatch(ThreadNum);
Mutex mutex;
long Countdown = 99999999;

void print( )
{
    LOG_INFO << "thread started";
    MainLatch.reduce(1);
    ThreadLatch.latch();

    while(true){
        MutexGuard lock(&mutex);
        if(Countdown)
            --Countdown;
        else
            break;
    }
    
    LOG_INFO << "thread end";
}

int main(int argc, const char * argv[]) {

    std::vector<thread_ptr> threads;
    for (int i = 0; i < ThreadNum; i++)
    {
        std::string threadName("Dummy-");
        threadName += boost::lexical_cast<std::string>(i);
        thread_func func = boost::bind(&print );
        thread_ptr t(new Thread(threadName, func)); 
        threads.push_back(t);
    }
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i]->start();
    }

    LOG_DEBUG << "main thread latch...";
    MainLatch.latch();
    LOG_DEBUG << "all dummy threads started...";
    ThreadLatch.reduce(1);
    
    //main thread will wait until all sub thread joined(end)
    for(int i=0; i<threads.size(); i++)
    {
        threads[i]->join();
    }

    assert(Countdown==0);
    
    return 0;
}

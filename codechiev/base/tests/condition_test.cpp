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

using namespace codechiev::base;

#define ThreadNum 10

CountLatch ThreadLatch(1);
CountLatch MainLatch(ThreadNum);

void print()
{
    Time::SleepMillis(100l);
    LOG_INFO << "started";
    MainLatch.reduce(1);
    ThreadLatch.latch();
    LOG_INFO << "end";
}

int main(int argc, const char * argv[]) {

    thread_func dummyFunc = boost::bind(&print);
    std::vector<thread_ptr> threads;
    for (int i = 0; i < ThreadNum; i++)
    {
        std::string threadName("Dummy-");
        threadName += boost::lexical_cast<std::string>(i);
        thread_ptr t(new Thread(threadName, dummyFunc)); 
        threads.push_back(t);
    }
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i]->start();
    }

    MainLatch.latch();
    printf("all dummy threads started...\n");
    Time::SleepMillis(1000l);
    ThreadLatch.reduce(1);

    //main thread will wait until all sub thread joined(end)
    for(int i=0; i<threads.size(); i++)
    {
        threads[i]->join();
    } 
    
    return 0;
}

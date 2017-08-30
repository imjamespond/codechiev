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
#include <vector>
#include <base/Logger.hpp>
#include <base/Thread.hpp>
#include <base/Time.hpp>
#include <base/Mutex.hpp>
#include <base/AtomicNumber.h>
#include <base/Condition.hpp>

using namespace codechiev::base;

int dummyCount = 0;
AtomicNumber<int64_t> atomicNum(0);
Mutex mutex;
const int kThreadNum = 10;
CountLatch latch(1);

void dummy()
{
    latch.latch();

    int count = 100000;
    while(count--)
    {
        dummyCount++;
    }

    LOG_INFO<<dummyCount;
}

void print()
{ 
    latch.latch();
    int count = 99999;
    while(count--)
    { atomicNum.fetchAndAdd(1); }

    LOG_INFO<<atomicNum.get();
}

int main(int argc, const char * argv[]) {

    thread_func dummyFunc = boost::bind(&dummy);
    std::vector<thread_ptr> threads;
    for(int i=0; i<kThreadNum; i++)
    {
        std::string tName("Dummy-");
        tName+=boost::lexical_cast<std::string>(i);
        thread_ptr t(new Thread(tName, dummyFunc));
        t->start();
        threads.push_back(t);
    }
    LOG_INFO<<"start dummies...";
    latch.reset(0);
    //main thread will wait until all sub thread joined(end)
    for(int i=0; i<threads.size(); i++)
    {
        threads[i]->join();
    }
    LOG_INFO<<dummyCount;
    
    thread_func printFunc = boost::bind(&print);
    threads.clear();
    for(int i=0; i<kThreadNum; i++)
    {
        std::string tName("Print-");
        tName+=boost::lexical_cast<std::string>(i);
        thread_ptr t(new Thread(tName, printFunc));
        t->start();
        threads.push_back(t);
    }
    latch.reset(0);
    LOG_INFO<<"start prints...";
    //main thread will wait until all sub thread joined(end)
    for(int i=0; i<threads.size(); i++)
    {
        threads[i]->join();
    }
    LOG_INFO<<atomicNum.get();
    return 0;
}

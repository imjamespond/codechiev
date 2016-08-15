//
//  main.cpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <stdio.h>
#include <boost/bind.hpp>
#include <vector>
#include <base/Logger.hpp>
#include <base/Thread.hpp>
#include <base/Time.hpp>
#include <base/Mutex.hpp>
#include <base/AtomicNumber.h>

using namespace codechiev::base;

AtomicNumber<int64_t> atomicNum(0);
Mutex mutex;
void print()
{
    int64_t count=0;
    while(atomicNum.fetchAndAdd(1)<9999999)
    {
        count++;
    }
    LOG_INFO<<count;
}

int main(int argc, const char * argv[]) {

    thread_func func = boost::bind(&print);
    std::vector<thread_ptr> threads;
    for(int i=0; i<10; i++)
    {
        thread_ptr t(new Thread("T", func));
        t->start();
        threads.push_back(t);
    }
    //main thread will wait until all sub thread joined(end)
    for(int i=0; i<threads.size(); i++)
    {
        threads[i]->join();
    }
    LOG_INFO<<atomicNum.addAndFetch(0);
    return 0;
}

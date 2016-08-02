//
//  main.cpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <stdio.h>
#include <boost/bind.hpp>
#include <base/Logger.hpp>
#include <base/Thread.hpp>
#include <base/Mutex.hpp>
#include <base/AtomicNumber.h>

using namespace codechiev::base;

AtomicNumber<int> atomicNum(0);
Mutex mutex;
void print()
{
    while(atomicNum.addFetch(1)<999999)
    {

    }
    LOG_INFO<<atomicNum.addFetch(0);
}

int main(int argc, const char * argv[]) {

    Thread::thread_func func = boost::bind(&print);

    Thread *threads[10];
    for(int i=0; i<(sizeof threads)/sizeof(Thread*); i++)
    {
        threads[i] = new Thread("T", func);
        threads[i]->start();
    }
    //main thread will wait until all sub thread joined(end)
    for(int i=0; i<(sizeof threads)/sizeof(Thread*); i++)
    {
        threads[i]->join();
    }

    return 0;
}

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

#include <base/BlockingQueue.hpp>
#include <base/Logger.hpp>
#include <base/AtomicNumber.h>
#include <base/Condition.hpp>
#include <base/Singleton.h>

using namespace codechiev::base;

class TestSingleton
{
public:
    TestSingleton(){count.addAndFetch(1);}
    
    static AtomicNumber<int> count;
};
class TestThreadSingleton
{
public:
    TestThreadSingleton(){count.addAndFetch(1);}

    static AtomicNumber<int> count;
};

const int kNumber = 99999;
CountLatch latch(kNumber<<1);
void SingletonTest()
{
    Singleton<TestSingleton>::get();
    latch.reduce(1);
}
void ThreadSingletonTest()
{
    ThreadSingleton<TestThreadSingleton>::get();
    latch.reduce(1);
}

AtomicNumber<int> TestSingleton::count(0);
AtomicNumber<int> TestThreadSingleton::count(0);

typedef BlockingQueue<10> bq;
bq queue;

int main(int argc, const char * argv[]) {
    
    BlockingQueue<10> queue;
    queue.commence();
    
    for(int i=0; i<kNumber; i++)
    {
        queue.addJob(boost::bind(&SingletonTest));
        queue.addJob(boost::bind(&ThreadSingletonTest));
    }
    
    latch.latch();
    LOG_INFO<<"TestSingleton:"<<TestSingleton::count.addAndFetch(0)\
    <<", TestThreadSingleton:"<<TestThreadSingleton::count.addAndFetch(0);
    queue.stop();
    
    return 0;
}

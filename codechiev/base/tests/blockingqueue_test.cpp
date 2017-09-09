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

#include "base/BlockingQueue.hpp"
#include "base/Logger.hpp"
#include "base/Random.hpp"
#include "base/Time.hpp"
#include "base/Logger.hpp"
#include <base/AtomicNumber.h>
#include <base/Condition.hpp>

using namespace codechiev::base;

AtomicNumber<int64_t> atomicNum(0);
CountLatch latch(1); 

void print()
{
    int64_t count = atomicNum.addAndFetch(1);
    if(count==999999)
        latch.reset(0);
}

int main(int argc, const char * argv[]) {
    
    Time begin = Time::Now();

    BlockingQueue<10> queue;
    queue.commence();
    
    for(int i=0; i<999999; i++)
    {
        queue.addJob(boost::bind(&print));
    } 
    
    latch.latch(); 
    Time now = Time::Now(); 
    LOG_INFO<<atomicNum.get()<<", cost:"<<now-begin;

    queue.addJob(boost::bind(&BlockingQueue<10>::stop, &queue));
    
    return 0;
}

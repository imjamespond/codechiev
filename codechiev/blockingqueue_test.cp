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
#include "base/Mutex.hpp"

using namespace codechiev::base;

int count(0);
void print()
{
    if(count)
    {
        count=0;
        if(count)
        {
            printf("count:%d\n",count);
        }
    }
    else
    {
        count++;
    }
    
    printf("current thread: %s - %d, %d\n",Thread::ThreadName().c_str(), Thread::GetTid(), errno);
}

int main(int argc, const char * argv[]) {
    
    BlockingQueue<10> queue;
    queue.commence();
    
    while(1)
    {
        BlockingQueue<10>::blocking_job job = boost::bind(&print);
        queue.addJob(job);
    }
    
    
    return 0;
}

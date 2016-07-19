//
//  main.cpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <stdio.h>
#include <boost/bind.hpp>

#include "base/Thread.hpp"
#include "base/Mutex.hpp"

using namespace codechiev::base;

int count(0);
Mutex mutex;
void print()
{
    while(1)
    {
        {
            //MutexGuard lock(&mutex);
            
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
        }
    }
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

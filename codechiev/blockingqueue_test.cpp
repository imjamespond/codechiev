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

#include <windows.h>

using namespace codechiev::base;

int count(0);
void print()
{
    if(count)
    {
        count=0;
        int64_t millis = random(100,110);
        Time::SleepMillis(millis);
        if(count)
        {
            //LOG_INFO<<Thread::ThreadName()<< "-" << Thread::GetTid()<<" sleep:"<<millis;
            throw std::runtime_error(std::string("count error..."));
        }
    }
    else
    {
        count++;
    }
}

int main(int argc, const char * argv[]) {

    BlockingQueue<10> queue;
    queue.commence();

    for(int i=0; i<999; i++)
    {
        queue.addJob(boost::bind(&print));
    }

    queue.addJob(boost::bind(&BlockingQueue<10>::stop, &queue));
    return 0;
}

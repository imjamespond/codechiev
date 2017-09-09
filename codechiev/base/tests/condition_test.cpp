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
#include <errno.h>

#include "base/Thread.hpp"
#include "base/Condition.hpp"
#include "base/Mutex.hpp"
#include "base/Time.hpp"
#include "base/Random.hpp"

using namespace codechiev::base;

Condition cond;
Mutex cond_mutex; 
int count(0);
bool running(true);
void print()
{
    while(running)
    {
        {
            MutexGuard lock(&cond_mutex);
            while(1)
            {
                cond.wait(cond_mutex);//release mutex and block here
                break;
            } 
        } 

        printf("thread wake: %s %d %d\n",
            Thread::ThreadName().c_str(), Thread::Tid(), count); 
    }
}

void wake(int ch)
{
    MutexGuard lock(&cond_mutex);
    
    if(ch=='a')
    { 
        cond.notifyall();
    }
    if(ch=='o')
    { 
        cond.notify();
    }
    if(ch=='q')
    {
        running=false;
        cond.notifyall();
    }
}

int main(int argc, const char * argv[]) {
    
    Thread* threads[8];
    
    for(int i=0;i<(sizeof(threads)/sizeof(Thread*));i++)
    {
        std::string name("thread-");
        name+=boost::lexical_cast<std::string>(i);
        Thread *t = new Thread(name, boost::bind(&print));//FIXME
        threads[i] = t;
        t->start();
    }
    
    
    int ch;
    do
    {
        ch=getchar();
        wake(ch);
        
    }while(ch!='.');
    
    
    for(int i=0;i<(sizeof(threads)/sizeof(Thread*));i++)
    {
        threads[i]->join();
    }
    return 0;
}

#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assert.hpp>
#include <vector>
#include <base/Logger.hpp>
#include <base/BlockedQueue.hpp>
#include <base/Singleton.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;

#define ThreadNum 10

long Countdown = 99999999;
Mutex mutexQuit;
CountLatch mainThreadLatch;

typedef struct count
{
    int i;
    count() : i(0) {}
} count_struct;

int print(int i)
{
    count_struct *count = ThreadSingleton<count_struct>::Get();
    if(++count->i % 100000 == 0)
        LOG_INFO<<count->i;
    if(i==Countdown)
        mainThreadLatch.unlatch();
    return 0;
}

int quit()
{
    MutexGuard lock(&mutexQuit); 
    count_struct *count = ThreadSingleton<count_struct>::Get();
    LOG_INFO<<count->i;
    Time::SleepMillis(500l);
    return 1;
}

int main(int argc, const char *argv[])
{
    SetLoggerLevel(Logger::Trace);
    BlockedQueue<8> queue;
    queue.start();
    for (int i = 0; i <= Countdown; ++i)
    {
        queue.add(boost::bind(&print, i));
    } 

    int size = queue.size();
    if(size)
    {
        LOG_INFO<<"queue size:"<<size; 
        mainThreadLatch.latch();
    }
    {
        MutexGuard lock(&mutexQuit);
        for(int i=0; i<8; ++i)
        {
            queue.add(boost::bind(&quit)); 
        }
    }

    // queue.stop();

    return 0;
}

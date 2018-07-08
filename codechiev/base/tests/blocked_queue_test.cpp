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

long Countdown = 999999;
BlockedQueue<ThreadNum> queue;

typedef struct count
{
    int i;
    count() : i(0) {}
} count_struct;

void print(int i)
{
    count_struct *count = ThreadSingleton<count_struct>::Get();
    if(++count->i % 100000 == 0)
        LOG_INFO<<count->i;
    if(i==Countdown)
    { 
        queue.stop();
    }
         
} 

int main(int argc, const char *argv[])
{
    SetLoggerLevel(Logger::Trace);
    queue.start();

    for (int i = 0; i <= Countdown; ++i)
    {
        queue.add(boost::bind(&print, i));
    }

    int size = queue.size();
    if(size)
    {
        LOG_INFO << "queue size:" << size; 
    } 
    queue.join();

    return 0;
}

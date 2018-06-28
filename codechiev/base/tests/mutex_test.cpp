#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assert.hpp>
#include <vector>
#include <base/Logger.hpp>
#include <base/Thread.hpp>
#include <base/Time.hpp> 
#include <base/Mutex.hpp>

using namespace codechiev::base;

#define ThreadNum 10

Mutex mutex;
Mutex mutex_;
Mutex mutex__(true);

void print()
{
    {
        MutexGuard lock(&mutex);
        MutexGuard lock_(&mutex_);
        Time::SleepMillis(1000l);
        LOG_INFO_R << "lock";
    }
    {
        MutexGuard lock(&mutex__);
        MutexGuard lock_(&mutex__);
        Time::SleepMillis(1000l);
        LOG_INFO_R << "recursive lock";
    }
}

int main(int argc, const char * argv[]) {

    thread_func dummyFunc = boost::bind(&print);
    std::vector<thread_ptr> threads;
    for (int i = 0; i < ThreadNum; i++)
    {
        std::string threadName("Dummy-");
        threadName += boost::lexical_cast<std::string>(i);
        thread_ptr t(new Thread(threadName, dummyFunc)); 
        threads.push_back(t);
    } 
    printf("start dummies...\n");
    Time::SleepMillis(100l); 

    for(int i=0; i<threads.size(); i++)
    {
        threads[i]->start();
    }
    //main thread will wait until all sub thread joined(end)
    for(int i=0; i<threads.size(); i++)
    {
        threads[i]->join();
    } 
    
    return 0;
}

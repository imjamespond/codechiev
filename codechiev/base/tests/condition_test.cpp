#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assert.hpp>
#include <vector>
#include <base/Logger.hpp>
#include <base/Thread.hpp>
#include <base/Time.hpp> 
#include <base/Condition.hpp>
#include <base/Mutex.hpp>

using namespace codechiev::base;

#define ThreadNum 10

CountLatch ThreadsLatch;
CountLatch MainThreadLatch(ThreadNum);
Mutex mutex;
long Countdown = 99999999;

void print( )
{
    LOG_INFO << MainThreadLatch.reduce(1);
    ThreadsLatch.latch(); 

    int count = 0;
    while(true){
        MutexGuard lock(&mutex);
        if(Countdown)
        {
            --Countdown;
            ++count;
        }else
            break;
    }

    LOG_INFO << "thread end, count:" << count;
}

int main(int argc, const char * argv[]) {

    std::vector<thread_ptr> threads;
    for (int i = 0; i < ThreadNum; i++)
    {
        std::string threadName("Dummy-");
        threadName += boost::lexical_cast<std::string>(i);
        thread_func func = boost::bind(&print );
        thread_ptr t(new Thread(threadName, func)); 
        threads.push_back(t);
    }
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i]->start();
    }

    MainThreadLatch.latch();
    ThreadsLatch.unlatch();
    LOG_DEBUG << "all threads started...";

    //main thread will wait until all sub thread joined(end)
    for(int i=0; i<threads.size(); i++)
    {
        threads[i]->join();
    }

    assert(Countdown==0);
    
    return 0;
}

#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assert.hpp>
#include <vector>
#include <base/Logger.hpp>
#include <base/Thread.hpp>
#include <base/Time.hpp>
#include <base/Mutex.hpp>
#include <base/Singleton.hpp>

using namespace codechiev::base;

#define ThreadNum 10


typedef struct test
{
    /* data */
    int val;
    test(int val):val(val){
        LOG_INFO << "test constructor";
    }
    ~test(){
        LOG_INFO << "test destructor";
    }
    static struct test *Instance()
    {
        return new struct test(3);
    }
} test_st;

void print()
{
    test_st* test = Singleton<test_st>::GetInstance();
    LOG_INFO << test->val;
}

int main(int argc, const char *argv[])
{

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

    for (int i = 0; i < threads.size(); i++)
    {
        threads[i]->start();
    }
    //main thread will wait until all sub thread joined(end)
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i]->join();
    }

    return 0;
}

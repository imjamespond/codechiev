#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assert.hpp>
#include <boost/unordered_map.hpp>
#include <vector>
#include <base/Logger.hpp>
#include <base/Thread.hpp>
#include <base/Time.hpp>
#include <base/UUID.hpp>
#include <base/Random.hpp>

using namespace codechiev::base;

#define ThreadNum 10

typedef boost::unordered_map<UUID::uuid_t, int> uuid_map;

void dummy(thread_ptr thread)
{
    int total = random(99999, 999999);
    LOG_INFO << "begin total: " << total;

    uuid_map uuidMap;
    for (int i = 0; i < 999999; ++i)
    {
        UUID::uuid_t session = UUID::GenUUID();
        uuidMap[session] = i;

        if (i % 10000 == 0)
        {
            std::string session_str = boost::lexical_cast<std::string>(session);
            LOG_INFO << session_str << "," << i;
        }

        if (i > total) 
        {
            LOG_INFO << "finish";
            thread->cancel();
        }
    }

}

int main(int argc, const char *argv[])
{

    std::vector<thread_ptr> threads;
    for (int i = 0; i < ThreadNum; i++)
    {
        std::string threadName("Dummy-");
        threadName += boost::lexical_cast<std::string>(i);
        thread_ptr t(new Thread(threadName));
        thread_func dummyFunc = boost::bind(&dummy, t);
        t->setFunc(dummyFunc);
        threads.push_back(t);
    }

    for (uint i = 0u; i < threads.size(); i++)
    {
        threads[i]->start();
    }

    Time::SleepMillis(3000l);

    for (uint i = 0u; i < threads.size(); i++)
    {
        threads[i]->cancel();
    }

    //main thread will wait until all sub thread joined(end)
    for (uint i = 0u; i < threads.size(); i++)
    {
        threads[i]->join();
    }

    return 0;
}

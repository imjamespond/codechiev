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

using namespace codechiev::base;

#define ThreadNum 10

void dummy()
{
    typedef boost::unordered_map<UUID::uuid_t, int> uuid_map;
    uuid_map uuidMap;

    LOG_INFO << "begin";
    for (int i = 0; i < 999999; ++i)
    {
        UUID::uuid_t session = UUID::GenUUID();
        uuidMap[session] = i;

        if (i % 10000 == 0)
        {
              std::string session_str = boost::lexical_cast<std::string>(session);
              LOG_INFO << session_str << "," <<i;
        }

    }
    LOG_INFO << "finish: " << uuidMap.size();
}

int main(int argc, const char *argv[])
{

    thread_func dummyFunc = boost::bind(&dummy);
    std::vector<thread_ptr> threads;
    for (int i = 0; i < ThreadNum; i++)
    {
        std::string threadName("Dummy-");
        threadName += boost::lexical_cast<std::string>(i);
        thread_ptr t(new Thread(threadName, dummyFunc));
        threads.push_back(t);
    }
    printf("start dummies...\n");
    Time::SleepMillis(500l);

    for (uint i = 0u; i < threads.size(); i++)
    {
        threads[i]->start();
    }
    //main thread will wait until all sub thread joined(end)
    for (uint i = 0u; i < threads.size(); i++)
    {
        threads[i]->join();
    }

    return 0;
}

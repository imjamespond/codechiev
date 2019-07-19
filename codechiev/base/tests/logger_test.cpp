#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <errno.h>

#include <base/Time.hpp>
#include <base/Thread.hpp>
#include <base/Logger.hpp>
#include <base/BlockedQueue.hpp>

using namespace codechiev::base; 

int statistic(int64_t costMillis)
{
    LOG_INFO << "main thread cost millis:" << costMillis;
    return 1;
}

int main(int argc, const char * argv[]) {
    
    std::string str("abc");
    double db=16000000.0/9.0;
    float ft=160.0/9.0;
    LOG_INFO << str
             << ", float:" << ft
             << ", int:" << 54321
             << ", double:" << db
             << KRED << " red" << KGRN << " green" << KYEL << " yellow" << KBLU << " blue" << KNRM
             << ", 111111111122222222222222233333333333333334444444444444555555555555556666666666666666667777777777777778888888888888888999999999999900000000000000";
    LOG_TRACE << str;
    LOG_DEBUG << str;
    LOG_WARN << str;

    // SetLoggerDetail(0);
    // int n;
    // for (n = 0; n < 3; n++)
    // {
    //     LOG_INFO_R<<n;
    //     sleep(1);
    // }
    // LOG_INFO_R<<"\n";

    Time before = Time::Now();
    for (int i = 0; i < 999; ++i)
    {
        STREAM_INFO << "stream logger:"<<i;
    }
    Time now = Time::Now();

    // LoggerStream::LoggerStreamQueue *Q = LoggerStream::GetQueue();
    // Q->add(boost::bind(&statistic, now - before));
    // Q->join();
    return 0;
}

#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <errno.h>

#include "base/Thread.hpp"
#include "base/Logger.hpp"

using namespace codechiev::base;

int main(int argc, const char * argv[]) {
    
    std::string str("abc");
    double db=16000000.0/9.0;
    float ft=160.0/9.0;
    LOG_INFO << str
             << ", float:" << ft
             << ", int:" << 54321
             << ", double:" << db
             << "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    LOG_TRACE << str;
    LOG_DEBUG << str;

    SetLoggerDetail(0);
    int n;
    for (n = 0; n < 3; n++)
    {
        LOG_INFO_R<<n;
        sleep(1);
    }
    LOG_INFO_R<<"\n";

    return 0;
}

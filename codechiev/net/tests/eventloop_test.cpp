//
//  main.cpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <stdio.h>
#include <boost/bind.hpp>
#include <errno.h>
#include <exception>
#include <net/EventLoop.h>
#include <net/Timer.hpp>
#include <base/Thread.hpp>
#include <base/Time.hpp>
#include <base/Logger.hpp>

using namespace codechiev;

class TestPoll
{
public:
    void poll(net::channel_vec& vec)
    {
        LOG_DEBUG<<"TestPoll::poll";
        base::Time::SleepMillis(1500l);
    }

};

void pollHandle(const net::channel_vec& vec)
{

}

int main(int argc, const char * argv[]) {

    typedef net::EventLoop<TestPoll> test_loop;
    test_loop loop;
    test_loop::loop_handle_func handle = boost::bind(&pollHandle, _1);
    base::Thread t("",boost::bind(&test_loop::loop, &loop, handle));
    t.start();
    t.join();
    return 0;
}

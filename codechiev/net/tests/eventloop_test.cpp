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

#include "net/EventLoop.h"
#include "net/EPoll.hpp"
#include "base/Thread.hpp"

using namespace codechiev;

class TestPoll
{
public:
    
    
};

void pollHandle()
{
    
}

int main(int argc, const char * argv[]) {
    
    typedef net::EventLoop<TestPoll> test_loop;
    test_loop loop(boost::bind(&pollHandle));
    base::Thread t("",boost::bind(&test_loop::loop, &loop));
    t.start();
    t.join();
    return 0;
}

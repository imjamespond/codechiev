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

int count(0);
void print()
{

}

int main(int argc, const char * argv[]) {
    
    typedef net::EventLoop<net::EPoll> epoll_loop;
    epoll_loop loop(boost::bind(&print));
    base::Thread t("",boost::bind(&epoll_loop::loop, &loop));
    t.start();
    t.join();
    return 0;
}

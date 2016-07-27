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
#include <net/Channel.hpp>
#include <net/EventLoop.h>
#include <net/EPoll.hpp>
#include <net/Timer.hpp>
#include <base/Thread.hpp>
#include <base/Logger.hpp>

using namespace codechiev;
typedef net::EventLoop<net::EPoll> epoll_loop;
int count(0);
void loopHandle(const net::channel_vec &vec)
{
    for( net::channel_vec::const_iterator it=vec.begin();
        it!=vec.end();
        it++)
    {
        net::Channel *channel = *it;
        typedef uint64_t data_t;
        data_t data(0);
        ssize_t len = ::read(channel->getFd(), &data, sizeof(data_t));//test level-trigger
        /*EINVAL fd was created via a call to timerfd_create(2) and the wrong
         size buffer was given to read(); see timerfd_create(2) for
         further information.*/
        LOG_DEBUG<<"read:"<<len<<", fd:"<<channel->getFd()<<", errno:"<<errno;
        if(len==sizeof(data_t))
            LOG_DEBUG<<"time's up";
    }
}

int main(int argc, const char * argv[]) {
    net::Timer timer;
    timer.after(3);
    
    epoll_loop loop(boost::bind(&loopHandle, _1));
    loop.getPoll().addChannel(&timer.getChannel());
    base::Thread t("",boost::bind(&epoll_loop::loop, &loop));
    t.start();
    t.join();
    return 0;
}

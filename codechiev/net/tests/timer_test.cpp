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
#include <base/Logger.hpp>
#include <net/Timer.hpp>
#include <net/EPoll.hpp>

using namespace codechiev;

int count(0);
void print()
{

}

int main(int argc, const char * argv[]) {



    /*test blocking fd
    uint64_t exp(0);
    while(1)
    {
        ssize_t len = ::read(timer.getChannel().getFd(), &exp, sizeof(uint64_t));
        //LOG_DEBUG<<"read";
        if(len==sizeof(uint64_t))
            LOG_DEBUG<<"time's up";
    }*/
    net::Channel::chanenl_vec vec;
    net::EPoll epoll;
    
    net::Timer timer;
    for(int i=0; i<3; i++)
    {
        timer.after(3);
        epoll.addEvent(timer.getChannel());
        epoll.wait(vec);
        
        for(net::Channel::chanenl_vec::iterator it=vec.begin();
            it!=vec.end();
            it++)
        {
            net::Channel *channel = *it;
            uint64_t exp(0);
            ssize_t len = ::read(channel->getFd(), &exp, sizeof(uint64_t));
            //LOG_DEBUG<<"read";
            if(len==sizeof(uint64_t))
                LOG_DEBUG<<"time's up";
        }
    }

    
    
    return 0;
}

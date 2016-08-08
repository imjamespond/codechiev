//
//  tcpserver_test.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//
#include <base/Thread.hpp>
#include <base/AtomicNumber.h>
#include <base/Logger.hpp>
#include <net/http/HttpServer.hpp>

#include <boost/bind.hpp>
#include <errno.h>

using namespace codechiev::base;
using namespace codechiev::net;

TcpServer serv("0.0.0.0", 9999);
AtomicNumber<int64_t> an(0);

void onMessage(Channel* channel)
{
    an.addAndFetch(channel->getReadBuf()->readable());
    channel->getReadBuf()->readall();
}

int main(int argc, const char * argv[]) {

    serv.setOnData(boost::bind(&onMessage, _1));
    
    Thread t("", boost::bind(&HttpServer::listen, &serv));
    t.start();

    int c(0);
    do
    {
        c=getchar();
        if(c == 10)
        {
        }
        else if(c == 'q')
        {
            t.cancel();
            break;
        }
        else if(c == 'p')
        {
            LOG_INFO<<serv.getChannelNum()<<", total reciev:"<<an.addAndFetch(0);
            an.set(0);
        }
    }while(c!='.');

    t.join();

    return 0;
}

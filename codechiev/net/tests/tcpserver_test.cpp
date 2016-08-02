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
#include <net/TcpEndpoint.hpp>

#include <boost/bind.hpp>
#include <errno.h>

using namespace codechiev::base;
using namespace codechiev::net;

TcpServer serv("0.0.0.0", 9999);
AtomicNumber<int64_t> an(0);
void onConnect(Channel* channel)
{
    LOG_DEBUG<<"onConnect fd:"<<channel->getFd();
}
void onMessage(Channel* channel)
{
    LOG_DEBUG<<"onMessage:"<<channel->getReadBuf()->str();
    serv.write(channel, channel->getReadBuf()->str());
    an.addAndFetch(1);
}
void onClose(Channel* channel)
{
    LOG_DEBUG<<"onClose fd:"<<channel->getFd();
}
int main(int argc, const char * argv[]) {
    serv.setOnConnect(boost::bind(&onConnect, _1));
    serv.setOnMessage(boost::bind(&onMessage, _1));
    serv.setOnClose(boost::bind(&onClose, _1));
    Thread t("", boost::bind(&TcpServer::listen, &serv));
    t.start();

    int c(0),i(0);
    do
    {
        c=getchar();
        if(c == 10)
        {
        }
        if('q')
        {
            t.cancel();
            break;
        }else if('p')
        {
            LOG_INFO<<serv.getChannelNum()<<",send"<<an.addAndFetch(0);
            an.set(0);
        }
''
    }while(c!='.');

    t.join();

    return 0;
}

//
//  tcpserver_test.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <base/Logger.hpp>
#include <net/TcpEndpoint.hpp>
#include <base/Thread.hpp>
#include <boost/bind.hpp>
#include <errno.h>

using namespace codechiev::base;
using namespace codechiev::net;

TcpServer serv("0.0.0.0", 9999);

void onConnect(Channel* channel)
{
    LOG_DEBUG<<"onConnect fd:"<<channel->getFd();
}
void onMessage(Channel* channel)
{
    LOG_DEBUG<<"onMessage:"<<channel->getReadBuf()->str();
    serv.write(channel, channel->getReadBuf()->str());
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
    t.join();

    return 0;
}

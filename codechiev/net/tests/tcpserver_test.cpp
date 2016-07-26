//
//  tcpserver_test.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <base/Logger.hpp>
#include <net/TcpServer.hpp>
#include <base/Thread.hpp>
#include <boost/bind.hpp>
#include <errno.h>

using namespace codechiev::base;
using namespace codechiev::net;

void onConnect(Channel* channel)
{
    LOG_DEBUG<<"onConnect fd:"<<channel->getFd()<<", errno:"<<errno;
}
void onMessage(const char* msg)
{
    LOG_DEBUG<<"onMessage:"<<msg;
}

int main(int argc, const char * argv[]) {

    TcpServer serv("0.0.0.0", 9999);
    Thread t("", boost::bind(&TcpServer::start, &serv));
    t.start();
    t.join();

    return 0;
}

//
//  tcpserver_test.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <stdio.h>
#include <base/Logger.hpp>
#include <net/TcpEndpoint.hpp>
#include <base/Thread.hpp>
#include <boost/bind.hpp>
#include <errno.h>

using namespace codechiev::base;
using namespace codechiev::net;

void onConnect(Channel* channel)
{
    LOG_DEBUG<<"onConnect fd:"<<channel->getFd();
}
void onMessage(const char* msg)
{
    LOG_DEBUG<<"onMessage:"<<msg;
}
void onClose(Channel* channel)
{
    LOG_DEBUG<<"onClose fd:"<<channel->getFd();
}
int main(int argc, const char * argv[]) {
    
    TcpClient client("127.0.0.1", 9999);
    client.setOnConnect(boost::bind(&onConnect,_1));
    client.setOnMessage(boost::bind(&onMessage,_1));
    client.setOnClose(boost::bind(&onClose,_1));
    Thread t("", boost::bind(&TcpClient::connect, &client));
    t.start();
    t.join();
    
    return 0;
}
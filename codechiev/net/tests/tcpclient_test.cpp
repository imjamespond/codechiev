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
    LOG_DEBUG<<"onConnect\n fd:"<<channel->getFd()<<\
    /*", sendbuf size:"<<channel->getSendBufSize()<<\
   ", setbuf size"<<channel->setSendBufSize(0)<<\ */
    ", sendbuf size:"<<channel->getSendBufSize();
}
void onMessage(Channel* channel)
{
    LOG_DEBUG<<"onMessage:"<<channel->getReadBuf()->str();
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

    int c(0),i(0);
    char msg[128];
    ::memset(msg, 0, sizeof msg);
    do
    {
        c=getchar();
        if(c == 10)
        {
            client.write(msg);
            i=0;
            ::memset(msg, 0, sizeof msg);
        }else
        {
            msg[i++]=c;
        }

    }while(c!='.');

    client.close();

    t.cancel();
    t.join();

    return 0;
}

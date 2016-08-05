//
//  EPoll.cpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "HttpServer.hpp"
#include <base/Logger.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <errno.h>

using namespace codechiev::net;

void onClose(Channel* channel)
{
    LOG_DEBUG<<"onClose fd:"<<channel->getFd();
}
void onConnect(Channel* channel)
{
    LOG_DEBUG<<"onConnect fd:"<<channel->getFd();
}

HttpServer::HttpServer(const std::string& ip, uint16_t port):
TcpServer(ip, port)
{
    setOnConnect(boost::bind(&::onConnect, _1));
    setOnData(boost::bind(&HttpServer::onMessage, this, _1));
    setOnClose(boost::bind(&::onClose, _1));
}

void
HttpServer::onMessage(Channel* channel)
{
    LOG_TRACE<<"onMessage:"<<channel->getReadBuf()->str()<<",r:"<<channel->getReadBuf()->reader()<<",w:"<<channel->getReadBuf()->writer();
    //serv.write(channel, channel->getReadBuf()->str());//echo
    
    while(1)
    {
        std::string httpMsg;
        const char* end = ::strstr(channel->getReadBuf()->str(), "\r\n\r\n");
        if(end)
        {
            int len = end-channel->getReadBuf()->str();
            httpMsg.append(channel->getReadBuf()->str(), len);
            LOG_DEBUG<<httpMsg<<",len:"<<len;
            channel->getReadBuf()->read(len+4);
            channel->getReadBuf()->move();
            
            //task
            //task(channel->getFd(), httpMsg)
            /*doTask()
             channel_ptr channel = serv.getChannel(fd)
             if(channel)
             channel->write(...)
             */
        }else
        {
            break;
        }
    }
    
    if(channel->getReadBuf()->writable()==0)
    {
        this->shut(channel);
    }
}

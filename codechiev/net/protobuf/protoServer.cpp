//
//  EPoll.cpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "ProtoServer.hpp"
#include <base/Logger.hpp>
#include <boost/bind.hpp>
#include <stdint.h>
#include <string.h>
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

ProtoServer::ProtoServer(const std::string& ip, uint16_t port):
TcpServer(ip, port)
{
    setOnConnect(boost::bind(&::onConnect, _1));
    setOnMessage(boost::bind(&ProtoServer::onMessage, this, _1));
    setOnClose(boost::bind(&::onClose, _1));
}

void
ProtoServer::onMessage(Channel* channel)
{
    LOG_TRACE<<"onMessage:"<<channel->getReadBuf()->str()<<",r:"<<channel->getReadBuf()->reader()<<",w:"<<channel->getReadBuf()->writer();
    //serv.write(channel, channel->getReadBuf()->str());//echo
    
    while(1)
    {
        int readable = channel->getReadBuf()->readable();
        
        if(readable>=4)
        {
            int32_t length,whole;
            ::memcpy(&length, channel->getReadBuf()->str(), sizeof(int32_t));
            whole = length + 4;
            if(readable>=whole)
            {
                std::string msg;
                msg.append(channel->getReadBuf()->str()+4, length);
                channel->getReadBuf()->read(whole);
            }
            else
            {
                return;
            }
        }
        else
        {
            return;
        }
    }
}

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
    setOnData(boost::bind(&ProtoServer::onData, this, _1));
    setOnClose(boost::bind(&::onClose, _1));
}

void
ProtoServer::onData(Channel* channel)
{
    LOG_TRACE<<"onData:"<<channel->getReadBuf()->str()<<",r:"<<channel->getReadBuf()->reader()<<",w:"<<channel->getReadBuf()->writer();
    //serv.write(channel, channel->getReadBuf()->str());//echo
    for(;;)
    {
        std::string msg;
        if(!tcplengthcoder::decode(channel, msg))
            break;
        queue_.addJob(boost::bind(&ProtoServer::onMessage, this, msg, channel->getFd()));
    }

}

void
ProtoServer::onMessage(const std::string& msg, int fd)
{
    LOG_TRACE<<"onMessage:"<<msg;
    channel_ptr channel = this->getChannel(fd);
}


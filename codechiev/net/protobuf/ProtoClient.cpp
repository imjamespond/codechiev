//
//  ProtoClient.cpp
//  codechie
//
//  Created by metasoft on 16/8/16.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "ProtoClient.hpp"

using namespace codechiev::base;
using namespace codechiev::net;


ProtoClient::ProtoClient(const std::string& ip, uint16_t port):
TcpClient(ip, port){}

void
ProtoClient::onData(Channel* channel)
{
    
    for(;;)
    {
        std::string msg;
        if(!tcplengthcoder::decode(channel, msg))
            break;
        LOG_TRACE<<"onData:"<<msg;
        onMessage( msg, channel);
    }
    
}

void
ProtoClient::onMessage(const std::string &msg, Channel *channel)
{
    onMessage_(msg);
}
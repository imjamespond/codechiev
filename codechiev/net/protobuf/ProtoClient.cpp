//
//  ProtoClient.cpp
//  codechie
//
//  Created by metasoft on 16/8/16.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "ProtoClient.hpp"
#include "TestPB.hpp"

using namespace codechiev::base;
using namespace codechiev::net;
using namespace com::codechiev::test;
using namespace google::protobuf;

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
    const GenericRsp &rspRef = GenericRsp::default_instance();
    
    GenericRsp *rsp = rspRef.New();
    rsp->ParseFromString(msg);
    LOG_INFO<<rsp->DebugString();
}
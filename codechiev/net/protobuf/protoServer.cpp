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

using namespace codechiev::base;
using namespace codechiev::net;
using namespace com::codechiev::test;
using namespace google::protobuf;

typedef boost::shared_ptr< Message > message_ptr;
typedef boost::shared_ptr<GenericReq> genericreq_ptr;

void onClose(Channel* channel)
{
    LOG_DEBUG<<"onClose fd:"<<channel->getFd();
}
void onConnect(Channel* channel)
{
    LOG_DEBUG<<"onConnect fd:"<<channel->getFd();
}

ProtoServer::ProtoServer(const std::string& ip, uint16_t port):
TcpServer(ip, port),onMessage_(0)
{
    setOnConnect(boost::bind(&::onConnect, _1));
    setOnData(boost::bind(&ProtoServer::onData, this, _1));
    setOnClose(boost::bind(&::onClose, _1));
    
    queue_.commence();
}

void
ProtoServer::onData(Channel* channel)
{
    
    for(;;)
    {
        std::string msg;
        if(!tcplengthcoder::decode(channel, msg))
            break;
        LOG_TRACE<<"onData:"<<msg;
        queue_.addJob(boost::bind(&ProtoServer::onMessage, this, msg, channel->getFd()));
    }

}

void
ProtoServer::onMessage(const std::string& msg, int fd)
{
    //channel_ptr channel = this->getChannel(fd);
    //if(onMessage_)
    //{
        //onMessage_(msg);
    //}
    const GenericReq &reqRef = GenericReq::default_instance();
    
    genericreq_ptr reqPtr(reqRef.New());
    reqPtr->ParseFromString(msg);
    LOG_INFO<<reqPtr->DebugString();
    
    const ServiceDescriptor *serviceDesc = service.GetDescriptor();
    const MethodDescriptor *method = serviceDesc->FindMethodByName( reqPtr->method());
    LOG_INFO<<serviceDesc->name();
    if(method)
    {
        message_ptr msgPtr(service.GetRequestPrototype(method).New());
        msgPtr->ParseFromString(reqPtr->request());
        LOG_INFO<<msgPtr->DebugString();
        Message *rsp = service.GetResponsePrototype(method).New();
        channel_ptr channel = getChannel(fd);
        rpc_send_func sendfunc = boost::bind(&ProtoServer::send, this, _1, _2);
        PbRpcChannel *rpcchannel = new PbRpcChannel(channel, sendfunc);
        Closure* callback = NewCallback(&PbRpcChannel::Callback, rpcchannel, rsp);
        service.CallMethod(method, NULL, msgPtr.get(), rsp, callback);
    }
}


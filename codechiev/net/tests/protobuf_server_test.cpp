//
//  tcpserver_test.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//
#include <base/Thread.hpp>
#include <base/AtomicNumber.h>
#include <base/Logger.hpp>
#include <net/protobuf/ProtoServer.hpp>
#include <boost/bind.hpp>
#include <errno.h>

#include "test.pb.h"

using namespace codechiev::base;
using namespace codechiev::net;

ProtoServer serv("0.0.0.0", 9999);
AtomicNumber<int64_t> an(0);

class TestService : public com::codechiev::test::NodeService
{
public:
};

void onMessage(const std::string& msg)
{
    const com::codechiev::test::GenericReq &reqRef = com::codechiev::test::GenericReq::default_instance();
    typedef boost::shared_ptr<com::codechiev::test::GenericReq> genericreq_ptr;
    genericreq_ptr genericReqPtr(reqRef.New());
    genericReqPtr->ParseFromString(msg);
    LOG_INFO<<genericReqPtr->DebugString();
    
    typedef boost::shared_ptr<::google::protobuf::Message> message_ptr;
    TestService service;
    const ::google::protobuf::ServiceDescriptor *serviceDesc = service.GetDescriptor();
    const ::google::protobuf::MethodDescriptor *methodDesc = serviceDesc->FindMethodByName( genericReqPtr->method());
    LOG_INFO<<serviceDesc->name();
    if(methodDesc)
    {
        message_ptr msgPtr(service.GetRequestPrototype(methodDesc).New());
        msgPtr->ParseFromString(genericReqPtr->request());
        LOG_INFO<<msgPtr->DebugString();
    }
    
}

int main(int argc, const char * argv[]) {
    
    serv.setOnMessage(boost::bind(&onMessage, _1));
    
    Thread t("", boost::bind(&ProtoServer::listen, &serv));
    t.start();
    
    int c(0);
    do
    {
        c=getchar();
        if(c == 10)
        {
        }
        else if(c == 'q')
        {
            t.cancel();
            break;
        }
        else if(c == 'p')
        {
            LOG_INFO<<serv.getChannelNum()<<",send"<<an.addAndFetch(0);
            an.set(0);
        }
    }while(c!='.');
    
    t.join();
    
    return 0;
}

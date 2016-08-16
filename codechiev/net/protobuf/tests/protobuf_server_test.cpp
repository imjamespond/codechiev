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
#include <net/protobuf/TestPB.hpp>
#include <boost/bind.hpp>
#include <errno.h>

using namespace codechiev::base;
using namespace codechiev::net;
using namespace com::codechiev::test;
using namespace google::protobuf;

ProtoServer serv("0.0.0.0", 9999);
AtomicNumber<int64_t> an(0);

TestServiceImpl service;
typedef boost::shared_ptr< Message > message_ptr;
typedef boost::shared_ptr<GenericReq> genericreq_ptr;

void testDone(Message *rsp)
{
    LOG_INFO<<"";
}
void onMessage(const std::string& msg)
{
    const GenericReq &reqRef = GenericReq::default_instance();
    
    genericreq_ptr genericReqPtr(reqRef.New());
    genericReqPtr->ParseFromString(msg);
    LOG_INFO<<genericReqPtr->DebugString();

    const ServiceDescriptor *serviceDesc = service.GetDescriptor();
    const MethodDescriptor *method = serviceDesc->FindMethodByName( genericReqPtr->method());
    LOG_INFO<<serviceDesc->name();
    if(method)
    {
        message_ptr msgPtr(service.GetRequestPrototype(method).New());
        msgPtr->ParseFromString(genericReqPtr->request());
        LOG_INFO<<msgPtr->DebugString();
        Message *rsp = service.GetResponsePrototype(method).New();

        Closure* callback = NewCallback(&testDone, rsp);
        service.CallMethod(method, NULL, msgPtr.get(), rsp, callback);
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

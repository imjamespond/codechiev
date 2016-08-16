//
//  tcpserver_test.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <base/Logger.hpp>
#include <base/BlockingQueue.hpp>
#include <base/Thread.hpp>

#include <boost/bind.hpp>
#include <errno.h>
#include <stdio.h>
#include <net/protobuf/Rpc.h>
#include <net/protobuf/TestPB.hpp>
#include <net/TcpEndpoint.hpp>
#include <net/TcpLengthCoder.h>
#include <net/protobuf/ProtoClient.hpp>

using namespace google::protobuf;
using namespace codechiev::base;
using namespace codechiev::net;
using namespace com::codechiev::test;

void DoSearch(const channel_ptr& channelPtr);

ProtoClient client("127.0.0.1", 9999);

int connNumber(1);
BlockingQueue<2> queue;
channel_ptr gChannel;
PbRpcChannel* rpcchannel;
TestService* service;
TestRequest request;
GenericRsp response;

void Done() {
    delete service;
    delete rpcchannel;
    //delete controller;
}

void DoSearch() {
    // You provide classes MyRpcChannel and MyRpcController, which implement
    // the abstract interfaces protobuf::RpcChannel and protobuf::RpcController.
    rpcchannel = new PbRpcChannel(gChannel, boost::bind(&ProtoClient::send, &client, _1, _2));
    //controller = new MyRpcController;
    
    // The protocol compiler generates the SearchService class based on the
    // definition given above.
    service = new TestService_Stub(rpcchannel);
    
    // Set up the request.
    request.set_id("12345");
    request.set_name("foobar");
    request.set_type(123);
    
    // Execute the RPC.
    service->RpcTest(NULL, &request, &response, NewCallback(&Done));
}


void onConnect(Channel* channel)
{
    LOG_DEBUG<<"onConnect\n fd:"<<channel->getFd()<<\
    /*", sendbuf size:"<<channel->getSendBufSize()<<\
     ", setbuf size"<<channel->setSendBufSize(0)<<\ */
    ", sendbuf size:"<<channel->getSendBufSize();
}

int main(int argc, const char * argv[]) {

    if((sizeof argc)>1)
    {
        connNumber=::atoi(argv[1]);
    }

    client.setOnConnect(boost::bind(&onConnect,_1));
    Thread t("", boost::bind(&ProtoClient::start, &client));
    t.start();/**/
    
    gChannel=client.connect();

    queue.commence();

    int c(0),i(0);
    char msg[128];
    ::memset(msg, 0, sizeof msg);
    do
    {
        c=getchar();
        if(c == 10)
        {
            DoSearch();
            i=0;
            ::memset(msg, 0, sizeof msg);
        }
        else
        {
            msg[i++]=c;
        }

    }while(c!='.');


    //t.cancel();
    //t.join();

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();


    return 0;
}

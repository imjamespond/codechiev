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
#include <net/TcpEndpoint.hpp>
#include <net/TcpLengthCoder.h>
#include <boost/bind.hpp>
#include <errno.h>
#include <stdio.h>
#include <net/protobuf/Rpc.h>
#include <net/protobuf/TestPB.hpp>

using namespace google::protobuf;
using namespace codechiev::base;
using namespace codechiev::net;
using namespace com::codechiev::test;
int connNumber(1);
BlockingQueue<2> queue;

PbRpcChannel* channel;
TestService* service;
TestRequest request;
GenericRsp response;

void Done() {
    delete service;
    delete channel;
    //delete controller;
}

void DoSearch(const channel_ptr& channelPtr) {
    // You provide classes MyRpcChannel and MyRpcController, which implement
    // the abstract interfaces protobuf::RpcChannel and protobuf::RpcController.
    channel = new PbRpcChannel(channelPtr);
    //controller = new MyRpcController;
    
    // The protocol compiler generates the SearchService class based on the
    // definition given above.
    service = new TestService_Stub(channel);
    
    // Set up the request.
    request.set_id("12345");
    request.set_name("foobar");
    
    // Execute the RPC.
    service->RpcTest(NULL, &request, &response, NewCallback(&Done));
}

class MultiClient : public TcpClient
{
public:
    MultiClient():TcpClient("127.0.0.1", 9999){}
    
    void onClose(Channel* channel)
    {
        TcpEndpoint::onClose(channel);
        channels.erase(channel->getFd());
    }
    
    void writetoall(const char* msg)
    {
        for(channel_map::const_iterator it=channels.begin();
            it!=channels.end();
            it++)
        {
            channel_ptr chn = it->second;
            //send(chn, msg);
            DoSearch(chn);
        }
    }
    
    void multiConnect(int num)
    {
        for(int i=0; i<num; i++)
        {
            channel_ptr chn = connect();
            if(chn)
                channels[chn->getFd()]=chn;
        }
        
        this->start();
    }
    
    void closeall()
    {
        for(channel_map::const_iterator it=channels.begin();
            it!=channels.end();
            it++)
        {
            channel_ptr chn = it->second;
            this->shut(chn.get());
        }
    }
    
    channel_map channels;
};
MultiClient client;

void onConnect(Channel* channel)
{
    LOG_DEBUG<<"onConnect\n fd:"<<channel->getFd()<<\
    /*", sendbuf size:"<<channel->getSendBufSize()<<\
     ", setbuf size"<<channel->setSendBufSize(0)<<\ */
    ", sendbuf size:"<<channel->getSendBufSize();
}
void onData(Channel* channel)
{
    LOG_DEBUG<<"onData:"<<channel->getReadBuf()->str();
}
void onClose(Channel* channel)
{
    LOG_DEBUG<<"onClose fd:"<<channel->getFd();
}

int main(int argc, const char * argv[]) {

    if((sizeof argc)>1)
    {
        connNumber=::atoi(argv[1]);
    }

    client.setOnConnect(boost::bind(&onConnect,_1));
    client.setOnData(boost::bind(&onData,_1));
    client.setOnClose(boost::bind(&onClose,_1));
    Thread t("", boost::bind(&MultiClient::multiConnect, &client));
    t.start();/**/

    queue.commence();

    int c(0),i(0);
    char msg[128];
    ::memset(msg, 0, sizeof msg);
    do
    {
        c=getchar();
        if(c == 10)
        {
            client.writetoall(msg);
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

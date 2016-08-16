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

int main(int argc, const char * argv[]) {


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

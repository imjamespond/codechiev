//
//  tcpserver_test.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <stdio.h>
#include <net/TcpServer.hpp>
#include <base/Thread.hpp>
#include <boost/bind.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

int main(int argc, const char * argv[]) {
    
    TcpServer serv("0.0.0.0", 9999);
    Thread t("", boost::bind(&TcpServer::start, &serv));
    t.start();
    t.join();
    
    return 0;
}
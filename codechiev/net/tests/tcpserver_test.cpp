//
//  tcpserver_test.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <stdio.h>
#include <net/TcpServer.hpp>

int main(int argc, const char * argv[]) {
    
    codechiev::net::TcpServer serv("0.0.0.0", 9999);
    serv.start();
    
    return 0;
}
//
//  TcpServer.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "TcpServer.hpp"
#include "EventLoop.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string>

#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)

using namespace codechiev::net;

TcpServer::TcpServer()
{
}

#define QUEUE_LIMIT 4
void
TcpServer::start()
{
    int sockfd;
    struct sockaddr addr;
    socklen_t socklen;
    
    sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    
    ::memset(&addr, 0, sizeof(sockaddr));
    //addr.sa_family =
    
    //The socket is bound to a local address
    ::bind(sockfd, &addr, socklen);
    
    //a queue limit for incoming connections
    ::listen(sockfd, QUEUE_LIMIT);
    
    loop.loop();
}
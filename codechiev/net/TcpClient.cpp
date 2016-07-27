//
//  TcpClient.cpp
//  codechiev
//
//  Created by metasoft on 16/7/27.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "TcpEndpoint.hpp"
#include <boost/bind.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpClient::TcpClient(const std::string& ip, uint16_t port):
TcpEndpoint(ip, port),
loop_(boost::bind(&TcpServer::pollEvent, this, _1))
{
    
}

void
TcpClient::connect()
{
    if (-1 == ::connect(<#int __fd#>, <#const struct sockaddr *__addr#>, <#socklen_t __len#>)(listench_.getFd(), (struct sockaddr *) &addr_.sockaddrin, addr_.socklen))
    {
        perror("bind error");
        LOG_ERROR<<"errno:"<<errno;
        exit(EXIT_FAILURE);
    }
}

void
TcpClient::close()
{
    
}
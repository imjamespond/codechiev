//
//  TcpServer.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "TcpServer.hpp"
#include "EventLoop.h"
#include <base/Logger.hpp>
#include <sys/types.h>
#include <string>
#include <errno.h>
#include <boost/bind.hpp>

using namespace codechiev::net;

TcpServer::TcpServer(const std::string& ip, uint16_t port):
listench_(::socket(AF_INET, SOCK_STREAM, 0)),
loop_(boost::bind(&TcpServer::pollEvent, this, _1)),
ipaddr_(ip),port_(port)
{
    if (listench_.getFd() == -1) {
        perror("socket error");
        LOG_ERROR<<"errno:"<<errno;
        exit(EXIT_FAILURE);
    }
    
    addrlen_ = sizeof(sockaddr_struct);
    LOG_DEBUG<<"create socket fd:"<<listench_.getFd();
}

#define QUEUE_LIMIT 4
void
TcpServer::start()
{
    ::memset(&addrin_, 0, sizeof(sockaddr));
    addrin_.sin_family = AF_INET;
    setIpAddress(ipaddr_, port_, addrin_);
    
    //The socket is bound to a local address
    ::bind(listench_.getFd(), (sockaddr_struct *) &addrin_, addrlen_);
    
    //a queue limit for incoming connections
    ::listen(listench_.getFd(), QUEUE_LIMIT);
    
    loop_.getPoll().addChannel(listench_);
    loop_.loop();
}

void
TcpServer::pollEvent(const chanenl_vec &vec)
{
    for( chanenl_vec::const_iterator it=vec.begin();
        it!=vec.end();
        it++)
    {
        net::Channel *channel = *it;
        if (channel->getFd() == listench_.getFd())
        {
            channel_ptr connsock(new Channel(::accept(listench_.getFd(),
                                                      (sockaddr_struct *) &addrin_, &addrlen_)));
            if (connsock->getFd() == -1) {
                LOG_ERROR<<("accept");
            }
            channels_[connsock->getFd()]=connsock;
            loop_.getPoll().addChannel(connsock);
            LOG_DEBUG<<"new connection fd:"<<channel->getFd()<<", errno:"<<errno;
        }
    }
}

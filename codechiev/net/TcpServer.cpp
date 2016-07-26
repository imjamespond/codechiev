//
//  TcpServer.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "TcpServer.hpp"
#include "EventLoop.h"
#include "Internet.h"
#include <base/Logger.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <errno.h>
#include <boost/bind.hpp>

using namespace codechiev::net;

TcpServer::TcpServer(const std::string& ip, uint16_t port):
servchannel_(::socket(AF_INET, SOCK_STREAM, 0)),
loop_(boost::bind(&TcpServer::pollEvent, this, _1)),
ipaddr_(ip),port_(port)
{
    if (servchannel_.getFd() == -1) {
        perror("socket ");
        LOG_ERROR<<"errno:"<<errno;
        exit(EXIT_FAILURE);
    }
}

#define QUEUE_LIMIT 4
void
TcpServer::start()
{
    struct sockaddr_in addr;
    ::memset(&addr, 0, sizeof(sockaddr));
    addr.sin_family = AF_INET;
    setIpAddress(ipaddr_, port_, addr);
    
    //The socket is bound to a local address
    ::bind(servchannel_.getFd(), (struct sockaddr *) &addr, sizeof(struct sockaddr));
    
    //a queue limit for incoming connections
    ::listen(servchannel_.getFd(), QUEUE_LIMIT);
    
    loop_.getPoll().addEvent(servchannel_);
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
        //typedef uint64_t data_t;
        //data_t data(0);
        //ssize_t len = ::read(channel->getFd(), &data, sizeof(data_t));//test level-trigger
        LOG_DEBUG<<"read:"<<0<<", fd:"<<channel->getFd()<<", errno:"<<errno;

    }
}

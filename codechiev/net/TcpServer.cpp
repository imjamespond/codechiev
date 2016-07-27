//
//  TcpServer.cpp
//  codechiev
//
//  Created by metasoft on 16/7/27.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "TcpEndpoint.hpp"

using namespace codechiev::base;
using namespace codechiev::net;

#define QUEUE_LIMIT 4
void
TcpServer::listen()
{
    if (-1 == listench_.setReuseAddr())
    {
        perror("setsockopt error");
        LOG_ERROR<<"errno:"<<errno;
        exit(EXIT_FAILURE);
    }
    
    //The socket is bound to a local address
    if (-1 == ::bind(listench_.getFd(), (struct sockaddr *) &addr_.sockaddrin, addr_.socklen))
    {
        perror("bind error");
        LOG_ERROR<<"errno:"<<errno;
        exit(EXIT_FAILURE);
    }
    
    //a queue limit for incoming connections
    if (-1 == ::listen(listench_.getFd(), QUEUE_LIMIT))
    {
        perror("listen error");
        LOG_ERROR<<"errno:"<<errno;
        exit(EXIT_FAILURE);
    }
    
    listench_.setEvent(EPOLLIN);
    loop_.getPoll().addChannel(&listench_);
    loop_.loop();
}

void
TcpServer::onConnect(Channel* channel)
{
    socklen_t socklen = addr_.socklen;
    int connfd = ::accept(listench_.getFd(),
                          (struct sockaddr *) &addr_.sockaddrin, &socklen);
    if (connfd == -1) {
        LOG_ERROR<<("accept");
        return;
    }
    channel_ptr connsock(new Channel(connfd));
    
#ifdef UseEpollET
    connsock->setEvent(EPOLLIN|EPOLLOUT |EPOLLET);
    LOG_TRACE<<"UseEpollET";
#else
    connsock->setEvent(EPOLLIN);
#endif
    channels_[connsock->getFd()]=connsock;
    loop_.getPoll().addChannel(connsock.get());
    if(onConnect_)
        onConnect_(connsock.get());
}

#undef UseEpollET
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
            onConnect(channel);
        }else
        {
            if(channel->getEvent() & EPOLLIN)
            {
                onRead(channel);
            }else if(channel->getEvent() & EPOLLOUT)
            {
                onWrite(channel);
            }else if(channel->getEvent() & (EPOLLHUP|EPOLLRDHUP) )
            {
                onClose(channel);
            }
        }
    }//for
}

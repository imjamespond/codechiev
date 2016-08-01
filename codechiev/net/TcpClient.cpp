//
//  TcpClient.cpp
//  codechiev
//
//  Created by metasoft on 16/7/27.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "TcpEndpoint.hpp"
#include "Channel.hpp"
#include <base/Logger.hpp>
#include <base/FixedBuffer.h>
#include <boost/bind.hpp>
#include <errno.h>

using namespace codechiev::base;
using namespace codechiev::net;

TcpClient::TcpClient(const std::string& ip, uint16_t port):
TcpEndpoint(ip, port)
{}

void
TcpClient::start()
{
    loop_.loop(boost::bind(&TcpClient::pollEvent, this, _1));
}

channel_ptr
TcpClient::connect()
{
    int connfd = ::socket(AF_INET, SOCK_STREAM| SOCK_NONBLOCK|SOCK_CLOEXEC, IPPROTO_TCP);
    if (connfd == -1)
    {
        perror("socket error");
        LOG_ERROR<<"errno:"<<errno;
        return channel_ptr(0);
    }
    channel_ptr chnptr(new Channel(connfd));

    if (-1 == ::connect(connfd, (struct sockaddr *) &addr_.sockaddrin, addr_.socklen))
    {
        if(EINPROGRESS == errno)
        {
            LOG_TRACE<<"waitting the nonblocking socket for writing";
        }else
        {
            perror("connect error");
            LOG_ERROR<<"errno:"<<errno;
            exit(EXIT_FAILURE);
        }
    }

    addChannel(chnptr.get(), EPOLLOUT);
}

void
TcpClient::pollEvent(const channel_vec &vec)
{
    for( channel_vec::const_iterator it=vec.begin();
        it!=vec.end();
        it++)
    {
        Channel *channel = *it;
        if(channel->getEvent() & EPOLLIN && onRead(channel))
        {
            continue;
        }
        if(channel->getEvent() & EPOLLOUT)
        {
            if(channel->isConnected())
            {
                if(onWrite(channel))
                    continue;
            }
            else
                onConnect(channel);
        }
        if(channel->getEvent() & (EPOLLHUP|EPOLLRDHUP) )
        {
            onClose(channel);
        }
    }//for
}

void
TcpClient::onConnect(Channel* channel)
{
#ifdef UseEpollET
    //channel->setEvent(EPOLLIN|EPOLLOUT |EPOLLET);
    LOG_TRACE<<"UseEpollET";
#endif
    channel->setKeepAlive();
    channel->setConnected(true);
    updateChannel(channel, EPOLLIN);
    if(onConnect_)
        onConnect_(channel);
}


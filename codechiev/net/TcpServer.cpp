//
//  TcpServer.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "TcpServer.hpp"
#include "EventLoop.h"
#include "Channel.hpp"
#include <base/Time.hpp>
#include <base/Logger.hpp>
#include <sys/types.h>
#include <string>
#include <errno.h>
#include <boost/bind.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpServer::TcpServer(const std::string& ip, uint16_t port):
listench_(::socket(AF_INET, SOCK_STREAM| SOCK_NONBLOCK|SOCK_CLOEXEC, 0)),
loop_(boost::bind(&TcpServer::pollEvent, this, _1)),
addr_(ip, port),
onConnect_(0),
onMessage_(0),
onClose_(0)
{
    if (listench_.getFd() == -1)
    {
        perror("socket error");
        LOG_ERROR<<"errno:"<<errno;
        exit(EXIT_FAILURE);
    }

    LOG_DEBUG<<"create socket fd:"<<listench_.getFd();
}

#define QUEUE_LIMIT 4
void
TcpServer::start()
{
    int on(1);
    if (::setsockopt(listench_.getFd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof on) == -1)
    {
        perror("setsockopt error");
        LOG_ERROR<<"errno:"<<errno;
        exit(EXIT_FAILURE);
    }

    //The socket is bound to a local address
    ::bind(listench_.getFd(), (struct sockaddr *) &addr_.sockaddrin, addr_.socklen);

    //a queue limit for incoming connections
    ::listen(listench_.getFd(), QUEUE_LIMIT);

    listench_.setEvent(EPOLLIN);
    loop_.getPoll().addChannel(&listench_);
    loop_.loop();
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

    Time::SleepMillis(2000l);
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
    connsock->setEvent(EPOLLIN|EPOLLOUT);
#endif
    channels_[connsock->getFd()]=connsock;
    loop_.getPoll().addChannel(connsock.get());
    if(onConnect_)
        onConnect_(connsock.get());
}

void
TcpServer::onClose(Channel* channel)
{
    loop_.getPoll().delChannel(channel);
    if(onClose_)
        onClose_(channel);
}

const int kBufferSize = 1024*1024;
const int kBufferHalfSize = kBufferSize*.5;
FixedBuffer<kBufferSize> buffer;//FIXME must be thread safe
void
TcpServer::onRead(Channel* channel)
{
    for(;;)
    {
        if(buffer.writable()<=kBufferHalfSize)
        {
            LOG_DEBUG<<buffer.str();
            buffer.readall();
        }
        int len = static_cast<int>(::read(channel->getFd(), buffer.data(), kBufferHalfSize));
        if(len)
        {
            buffer.write(len);
        }else
        {
            onClose(channel);
            break;
        }
        //reading done
        if(EAGAIN==errno)
        {
            if(onMessage_&&buffer.readable())
                onMessage_(buffer.str());
            buffer.readall();
#ifndef UseEpollET
            //set channel being interesting in read event
            channel->setEvent(EPOLLIN);//edge-trigger don't need
            loop_.getPoll().setChannel(channel);
#endif
            break;
        }
    }//for
}

void
TcpServer::onWrite(Channel* channel)
{
#ifndef UseEpollET
    //set channel being interesting in read event
    channel->setEvent(EPOLLIN);//edge-trigger don't need
    loop_.getPoll().setChannel(channel);
#endif
}


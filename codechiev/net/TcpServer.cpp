//
//  TcpEndpoint.cpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "TcpEndpoint.hpp"
#include "EventLoop.h"
#include "Channel.hpp"
#include <base/Time.hpp>
#include <base/Logger.hpp>
#include <base/FixedBuffer.h>
#include <sys/types.h>
#include <string>
#include <errno.h>
#include <boost/bind.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpEndpoint::TcpEndpoint(const std::string& ip, uint16_t port):
addr_(ip, port),
channel_(::socket(AF_INET, SOCK_STREAM| SOCK_NONBLOCK|SOCK_CLOEXEC, 0)),
onConnect_(0),
onMessage_(0),
onClose_(0)
{
    if (channel_.getFd() == -1)
    {
        perror("socket error");
        LOG_ERROR<<"errno:"<<errno;
        exit(EXIT_FAILURE);
    }

    LOG_DEBUG<<"TcpEndpoint fd:"<<channel_.getFd();
}

TcpServer::TcpServer(const std::string& ip, uint16_t port):
TcpEndpoint(ip, port),
loop_(boost::bind(&TcpServer::pollEvent, this, _1))
{

}

#define QUEUE_LIMIT 4
void
TcpServer::listen()
{
    if (-1 == channel_.setReuseAddr())
    {
        perror("setsockopt error");
        LOG_ERROR<<"errno:"<<errno;
        exit(EXIT_FAILURE);
    }

    //The socket is bound to a local address
    if (-1 == ::bind(channel_.getFd(), (struct sockaddr *) &addr_.sockaddrin, addr_.socklen))
    {
        perror("bind error");
        LOG_ERROR<<"errno:"<<errno;
        exit(EXIT_FAILURE);
    }

    //a queue limit for incoming connections
    if (-1 == ::listen(channel_.getFd(), QUEUE_LIMIT))
    {
        perror("listen error");
        LOG_ERROR<<"errno:"<<errno;
        exit(EXIT_FAILURE);
    }

    updateChannel(channel, EPOLLIN);
    loop_.loop();
}

void
TcpServer::pollEvent(const channel_vec &vec)
{

    for( channel_vec::const_iterator it=vec.begin();
        it!=vec.end();
        it++)
    {
        net::Channel *channel = *it;
        if (channel->getFd() == channel_.getFd())
        {
            onConnect(channel);
        }else
        {
            if(channel->getEvent() & EPOLLIN && onRead(channel))
            {
                continue;
            }
            if(channel->getEvent() & EPOLLOUT && onWrite(channel))
            {
                continue;
            }
            if(channel->getEvent() & (EPOLLHUP|EPOLLRDHUP) )
            {
                onClose(channel);
            }
        }
    }//for

    //Time::SleepMillis(10000l);//simulate combine event when using et
}

void
TcpServer::onConnect(Channel* channel)
{
    socklen_t socklen = addr_.socklen;
    InetAddress addr;
    int connfd = ::accept(channel_.getFd(),
                          (struct sockaddr *) &addr.sockaddrin, &socklen);
    if (connfd == -1) {
        LOG_ERROR<<("accept");
        return;
    }
    channel_ptr connsock(new Channel(connfd));

#ifdef UseEpollET
    //connsock->setEvent(EPOLLIN|EPOLLOUT |EPOLLET);
    LOG_TRACE<<"UseEpollET";
#endif
    connsock->setEvent(EPOLLIN);
    loop_.getPoll().addChannel(connsock.get());
    channels_[connsock->getFd()]=connsock;
    if(onConnect_)
        onConnect_(connsock.get());
}

void
TcpServer::onClose(Channel* channel)
{
    if(onClose_)
        onClose_(channel);
    loop_.getPoll().delChannel(channel);
    channel->close();
}

bool
TcpServer::onRead(Channel* channel)
{
    char buffer[kBufferEachTimeSize];
    for(;;)
    {
        ::memset(buffer, '\0', sizeof buffer);
        int len = static_cast<int>(::read(channel->getFd(), buffer, kBufferEachTimeSize));
        channel->getReadBuf()->append(buffer, len);
        LOG_TRACE<<"read:"<<len<<",errno:"<<errno;

        if(len==0)
        {
            onClose(channel);
            return true;
        }
        //reading done
        if(EAGAIN==errno)
        {
            updateChannel(channel, EPOLLIN);

            if(onMessage_&&channel->getReadBuf()->readable())
            {
                onMessage_(channel->getReadBuf()->str());
                write(channel, "abcdefghijklmnopqrstuvwxyz1234567890");
            }

            channel->getReadBuf()->readall();
            return false;
        }
    }//for
}

bool
TcpServer::onWrite(Channel* channel)
{
    for(;;)
    {
        int readable = channel->getWriteBuf()->readable();
        if(readable > kBufferEachTimeSize)
        {
            readable = kBufferEachTimeSize;
        }
        int len = static_cast<int>(::write(channel->getFd(), channel->getWriteBuf()->str(), readable));
        LOG_TRACE<<"write:"<<len;
        if(len)
        {
            channel->getWriteBuf()->read(len);
        }
        else if(len==0)
        {
            channel->writeEvent();
            channel->getWriteBuf()->readall();

            updateChannel(channel, EPOLLIN);
            return false;
        }

        if(EAGAIN==errno)
        {
            LOG_TRACE<<"EAGAIN";
            if(channel->getWriteBuf()->readable())
            {
                updateChannel(channel, EPOLLIN|EPOLLOUT);
            }else
            {
                updateChannel(channel, EPOLLIN);
            }
            loop_.getPoll().setChannel(channel);

            channel->writeEvent();
            return false;
        }
    }
}

void
TcpServer::write(Channel *channel, const std::string& msg)
{
    channel->write(msg);
    if(channel->getWriteBuf()->readable())
    {
        channel->setEvent(EPOLLIN|EPOLLOUT);
        loop_.getPoll().setChannel(channel);
    }
}

void
TcpServer::updateChannel(Channel *channel, int events)
{
    channel->setEvent(events);
    loop_.getPoll().setChannel(channel);
}


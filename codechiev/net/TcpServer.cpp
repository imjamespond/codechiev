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
#include <base/FixedBuffer.h>
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

    LOG_DEBUG<<"TcpServer fd:"<<listench_.getFd();
}

#define QUEUE_LIMIT 4
void
TcpServer::start()
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

void
TcpServer::onClose(Channel* channel)
{
    if(onClose_)
        onClose_(channel);
    loop_.getPoll().delChannel(channel);
    channel->close();
}

const int kReadBufferSize = 1024*1024;
const int kReadBufferEachTimeSize = 16;
FixedBuffer<kReadBufferSize> readbuf;//FIXME must be thread safe
void
TcpServer::onRead(Channel* channel)
{
    for(;;)
    {
        if(readbuf.writable()<=kReadBufferEachTimeSize)
        {
            LOG_ERROR<<"insufficient buffer:"<<readbuf.str();
            readbuf.readall();
        }
        ssize_t len = static_cast<int>(::read(channel->getFd(), readbuf.data(), kReadBufferEachTimeSize));
        LOG_TRACE<<"read:"<<len;
        if(len)
        {
            readbuf.write(static_cast<int>(len));
        }else
        {
            onClose(channel);
            break;
        }
        //reading done
        if(EAGAIN==errno)
        {
            if(onMessage_&&readbuf.readable())
            {
                onMessage_(readbuf.str());
                //write(channel, "Since even with edge-triggered epoll, multiple events can be\
                      generated upon receipt of multiple chunks of data, the caller has the\
                      option to specify the EPOLLONESHOT flag, to tell epoll to disable the\
                      associated file descriptor after the receipt of an event withepoll_wait(2).When the EPOLLONESHOT flag is specified,it is thecaller\'s responsibility to rearm the file descriptor usingepoll_ctl(2) with EPOLL_CTL_MOD.Interaction with autosleep\
                      If the system is in autosleep mode via /sys/power/autosleep and an\
                      event happens which wakes the device from sleep, the device driver\
                      will keep the device awake only until that event is queued.  To keep\
                      the device awake until the event has been processed, it is necessary");
            }
            
            readbuf.readall();
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
    for(;;)
    {
        ssize_t len = ::write(channel->getFd(), channel->getWriteBuf().str(), channel->getWriteBuf().readable());
        LOG_TRACE<<"write:"<<len;
        if(len)
        {
            channel->getWriteBuf().read(len);
        }
        
        if(EAGAIN==errno)
        {
#ifndef UseEpollET
            if(channel->getWriteBuf().readable())
            {
                channel->setEvent(EPOLLOUT);
            }else
            {
                channel->setEvent(EPOLLIN);
            }
            loop_.getPoll().setChannel(channel);
#endif
            break;
        }
    }
}

void
TcpServer::write(Channel *channel, const std::string& msg)
{
    channel->write(msg);
    
#ifndef UseEpollET
    //set channel being interesting in write event
    channel->setEvent(EPOLLOUT);//edge-trigger don't need
    loop_.getPoll().setChannel(channel);
#endif
    
}


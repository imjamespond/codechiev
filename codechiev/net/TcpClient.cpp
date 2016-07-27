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
TcpEndpoint(ip, port),
loop_(boost::bind(&TcpClient::pollEvent, this, _1)),
connected_(false)
{}

void
TcpClient::connect()
{
    if (-1 == ::connect(channel_.getFd(), (struct sockaddr *) &addr_.sockaddrin, addr_.socklen))
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
    
    channel_.setKeepAlive();
    channel_.setEvent(EPOLLOUT);
    loop_.getPoll().addChannel(&channel_);
    loop_.loop();
}

void
TcpClient::close()
{
    
}

void
TcpClient::pollEvent(const channel_vec &vec)
{
    
    for( channel_vec::const_iterator it=vec.begin();
        it!=vec.end();
        it++)
    {
        net::Channel *channel = *it;
        if (channel->getFd() == channel_.getFd())
        {
            
            if(channel->getEvent() & EPOLLIN)
            {
                onRead(channel);
            }else if(channel->getEvent() & EPOLLOUT)
            {
                if(connected_)
                    onWrite(channel);
                else
                    onConnect(channel);
            }else if(channel->getEvent() & (EPOLLHUP|EPOLLRDHUP) )
            {
                onClose(channel);
            }
        }
    }//for
}

void
TcpClient::onConnect(Channel* channel)
{
    connected_ = true;
#ifdef UseEpollET
    channel->setEvent(EPOLLIN|EPOLLOUT |EPOLLET);
    LOG_TRACE<<"UseEpollET";
#else
    channel->setEvent(EPOLLIN);
#endif
    
    loop_.getPoll().setChannel(channel);
    if(onConnect_)
        onConnect_(channel);
}


void
TcpClient::onClose(Channel* channel)
{
    if(onClose_)
        onClose_(channel);
    loop_.getPoll().delChannel(channel);
    channel->close();
}

void
TcpClient::onRead(Channel* channel)
{
    for(;;)
    {
        if(channel->getReadBuf()->writable()<=kBufferEachTimeSize)
        {
            LOG_ERROR<<"insufficient buffer:"<<channel->getReadBuf()->str();
            channel->getReadBuf()->readall();
        }
        ssize_t len = static_cast<int>(::read(channel->getFd(), channel->getReadBuf()->data(), kBufferEachTimeSize));
        LOG_TRACE<<"read:"<<len;
        if(len)
        {
            channel->getReadBuf()->write(static_cast<int>(len));
        }else
        {
            onClose(channel);
            break;
        }
        //reading done
        if(EAGAIN==errno)
        {
            
#ifndef UseEpollET
            channel->setEvent(EPOLLIN);
            loop_.getPoll().setChannel(channel);
#endif
            
            if(onMessage_&&channel->getReadBuf()->readable())
            {
                onMessage_(channel->getReadBuf()->str());
            }
            
            channel->getReadBuf()->readall();
            break;
        }
    }//for
}

void
TcpClient::onWrite(Channel* channel)
{
    for(;;)
    {
        int readable = channel->getWriteBuf()->readable();
        int len = static_cast<int>(::write(channel->getFd(), channel->getWriteBuf()->str(), readable));
        LOG_TRACE<<"write:"<<len;
        if(readable==len)
        {
            channel->writeEvent();
            channel->getWriteBuf()->readall();
            break;
        }
        else if(len)
        {
            channel->getWriteBuf()->read(len);
        }
        
        if(EAGAIN==errno)
        {
            channel->writeEvent();
            break;
        }
    }
#ifndef UseEpollET
    if(channel->wreadable())
    {
        channel->setEvent(EPOLLOUT);
    }else
    {
        channel->setEvent(EPOLLIN);
    }
    loop_.getPoll().setChannel(channel);
#endif
}

void
TcpClient::write(const std::string& msg)
{
    channel_->write(msg);
    if(channel_.getWriteBuf()->readable())
    {
#ifndef UseEpollET
        channel_.setEvent(EPOLLOUT);
        loop_.getPoll().setChannel(&channel_);
#endif
    }
}

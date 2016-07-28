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
            if(channel->getEvent() & EPOLLIN && onRead(channel))
            {
                continue;
            }
            if(channel->getEvent() & EPOLLOUT)
            {
                if(connected_&&onWrite(channel))
                {
                    continue;
                }
                else
                    onConnect(channel);
                onWrite(channel);
                
            }
            if(channel->getEvent() & (EPOLLHUP|EPOLLRDHUP) )
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
    //channel->setEvent(EPOLLIN|EPOLLOUT |EPOLLET);
    LOG_TRACE<<"UseEpollET";
#endif
    channel->setEvent(EPOLLIN);
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
            onClose(channel);
            break;
        }
        int len = static_cast<int>(::read(channel->getFd(), channel->getReadBuf()->data(), kBufferEachTimeSize));
        LOG_TRACE<<"read:"<<len;
        if(len)
        {
            channel->getReadBuf()->write(len);
        }else if(len==0)
        {
            onClose(channel);
            break;
        }
        //reading done
        if(EAGAIN==errno)
        {
            channel->setEvent(EPOLLIN);
            loop_.getPoll().setChannel(channel);
            
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
            
            channel->setEvent(EPOLLIN);
            loop_.getPoll().setChannel(channel);
            break;
        }
        
        if(EAGAIN==errno)
        {
            if(channel->getWriteBuf()->readable())
            {
                channel->setEvent(EPOLLIN|EPOLLOUT);
            }else
            {
                channel->setEvent(EPOLLIN);
            }
            loop_.getPoll().setChannel(channel);
            channel->writeEvent();
            break;
        }
    }
}

void
TcpClient::write(const std::string& msg)
{
    channel_.write(msg);
    if(channel_.getWriteBuf()->readable())
    {
        channel_.setEvent(EPOLLIN|EPOLLOUT);
        loop_.getPoll().setChannel(&channel_);
    }
}

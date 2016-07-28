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
loop_(boost::bind(&TcpClient::pollEvent, this, _1))
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
    updateChannel(&channel_, EPOLLOUT);
    loop_.loop();
}

void
TcpClient::close()
{
    Channel *channel=&channel_;
    if(onClose_)
        onClose_(channel);
    loop_.getPoll().delChannel(channel);
    channel->close();
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
    channel->setConnected(true);
    updateChannel(channel, EPOLLIN);
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

bool
TcpClient::onRead(Channel* channel)
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
            }

            channel->getReadBuf()->readall();
            return false;
        }
    }//for
}

bool
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

            updateChannel(channel, EPOLLIN|EPOLLOUT);
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
TcpClient::write(const std::string& msg)
{
    channel_.write(msg);
    if(channel_.getWriteBuf()->readable())
    {
        updateChannel(&channel_, EPOLLIN|EPOLLOUT);
    }
}

void
TcpClient::updateChannel(Channel *channel, int events)
{
    channel->setEvent(events);
    loop_.getPoll().setChannel(channel);
}

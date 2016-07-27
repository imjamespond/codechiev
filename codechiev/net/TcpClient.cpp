//
//  TcpClient.cpp
//  codechiev
//
//  Created by metasoft on 16/7/27.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "TcpEndpoint.hpp"
#include <base/Logger.hpp>
#include <boost/bind.hpp>
#include <errno.h>

using namespace codechiev::base;
using namespace codechiev::net;

TcpClient::TcpClient(const std::string& ip, uint16_t port):
TcpEndpoint(ip, port),
loop_(boost::bind(&TcpClient::pollEvent, this, _1))
{
    
}

void
TcpClient::connect()
{
    if (-1 == ::connect(channel_.getFd(), (struct sockaddr *) &addr_.sockaddrin, addr_.socklen))
    {
        perror("bind error");
        LOG_ERROR<<"errno:"<<errno;
        exit(EXIT_FAILURE);
    }
}

void
TcpClient::close()
{
    
}

#undef UseEpollET
void
TcpClient::pollEvent(const chanenl_vec &vec)
{
    
    for( chanenl_vec::const_iterator it=vec.begin();
        it!=vec.end();
        it++)
    {
        net::Channel *channel = *it;
        if (channel->getFd() == channel_.getFd())
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
TcpClient::onConnect(Channel* channel)
{
    
#ifdef UseEpollET
    channel->setEvent(EPOLLIN|EPOLLOUT |EPOLLET);
    LOG_TRACE<<"UseEpollET";
#else
    channel->setEvent(EPOLLIN);
#endif
}


void
TcpClient::onClose(Channel* channel)
{
    if(onClose_)
        onClose_(channel);
    loop_.getPoll().delChannel(channel);
    channel->close();
}

const int kReadBufferSize = 1024*1024;
const int kReadBufferEachTimeSize = 16;
FixedBuffer<kReadBufferSize> clientbuf;//FIXME must be thread safe
void
TcpClient::onRead(Channel* channel)
{
    for(;;)
    {
        if(clientbuf.writable()<=kReadBufferEachTimeSize)
        {
            LOG_ERROR<<"insufficient buffer:"<<clientbuf.str();
            clientbuf.readall();
        }
        ssize_t len = static_cast<int>(::read(channel->getFd(), clientbuf.data(), kReadBufferEachTimeSize));
        LOG_TRACE<<"read:"<<len;
        if(len)
        {
            clientbuf.write(static_cast<int>(len));
        }else
        {
            onClose(channel);
            break;
        }
        //reading done
        if(EAGAIN==errno)
        {
            
#ifndef UseEpollET
            //set channel being interesting in read event
            channel->setEvent(EPOLLIN);//edge-trigger don't need
            loop_.getPoll().setChannel(channel);
#endif
            
            if(onMessage_&&clientbuf.readable())
            {
                onMessage_(clientbuf.str());
            }
            
            clientbuf.readall();
            break;
        }
    }//for
}

void
TcpClient::onWrite(Channel* channel)
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
            
            channel->writeEvent();
            break;
        }
    }
}

void
TcpClient::write(Channel *channel, const std::string& msg)
{
    channel->write(msg);
    
#ifndef UseEpollET
    //set channel being interesting in write event
    channel->setEvent(EPOLLOUT);//edge-trigger don't need
    loop_.getPoll().setChannel(channel);
#endif
    
}

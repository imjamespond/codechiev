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
#include <base/FixedBuffer.h>
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
ipaddr_(ip),port_(port),
onConnect_(0),
onMessage_(0),
onClose_(0)
{
    if (listench_.getFd() == -1) {
        perror("socket error");
        LOG_ERROR<<"errno:"<<errno;
        exit(EXIT_FAILURE);
    }

    addrlen_ = sizeof(sockaddr_struct);
    LOG_DEBUG<<"create socket fd:"<<listench_.getFd();
}

#define QUEUE_LIMIT 4
void
TcpServer::start()
{
    ::memset(&addrin_, 0, sizeof(sockaddr));
    addrin_.sin_family = AF_INET;
    setIpAddress(ipaddr_, port_, addrin_);

    //The socket is bound to a local address
    ::bind(listench_.getFd(), (sockaddr_struct *) &addrin_, addrlen_);

    //a queue limit for incoming connections
    ::listen(listench_.getFd(), QUEUE_LIMIT);

    listench_->setEvent(EPOLLIN);
    loop_.getPoll().addChannel(&listench_);
    loop_.loop();
}

const int kBufferSize = 1024*1024;
const int kBufferHalfSize = kBufferSize*.5;
void
TcpServer::pollEvent(const chanenl_vec &vec)
{
    FixedBuffer<kBufferSize> buffer;
Time::SleepMillis(2000l);
    for( chanenl_vec::const_iterator it=vec.begin();
        it!=vec.end();
        it++)
    {
        net::Channel *channel = *it;
        LOG_TRACE<<"event:"<<channel->getEvent();
        if (channel->getFd() == listench_.getFd())
        {
            channel_ptr connsock(new Channel(::accept(listench_.getFd(),
                                                      (sockaddr_struct *) &addrin_, &addrlen_)));
            if (connsock->getFd() == -1) {
                LOG_ERROR<<("accept");
            }
            //#undef UseEpollET
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
        }else
        {
            if(channel->getEvent() & EPOLLIN)
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
                }
                if(EAGAIN==errno)
                {
                    if(onMessage_&&buffer.readable())
                        onMessage_(buffer.str());
                    buffer.readall();
                    //set channel being interesting in read event
                    //channel->setEvent(EPOLLIN);//edge-trigger don't need
                    //loop_.getPoll().setChannel(channel);
                    break;
                }
            }
            }else if(channel->getEvent() & EPOLLOUT)
            {
                //channel->setEvent(EPOLLIN);
                //loop_.getPoll().setChannel(channel);
            }else if(channel->getEvent() & (EPOLLHUP|EPOLLRDHUP) )
            {
                loop_.getPoll().delChannel(channel);
            }

        }
    }
}

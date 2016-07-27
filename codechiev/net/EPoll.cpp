//
//  EPoll.cpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "EPoll.hpp"
#include <base/Logger.hpp>
#include <cstdlib>
#include <errno.h>

using namespace codechiev::net;

#define MAX_EVENTS 10
EPoll::EPoll():epollch_(::epoll_create1(EPOLL_CLOEXEC)),
events_(MAX_EVENTS)
{
    if (epollch_.getFd() == -1)
    {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
}

void
EPoll::addChannel(Channel *channel)
{
    struct epoll_event ev;

    ev.events = channel->getEvent();
    ev.data.fd = epollch_.getFd();
    ev.data.ptr = channel;

    channel->setNonBlock();

    if(events_.size()==MAX_EVENTS)
    {
        events_.resize(events_.size()<<1);//double size
    }
    if (::epoll_ctl(epollch_.getFd(), EPOLL_CTL_ADD, channel->getFd(), &ev) == -1)
    {
        perror("epoll_ctl: listen_sock");
        LOG_ERROR<<"errno:"<<errno;
    }
}

void
EPoll::setChannel(Channel *channel)
{
    struct epoll_event ev;
    ev.events = channel->getEvent();

    ev.data.fd = epollch_.getFd();
    ev.data.ptr = channel;

    if (::epoll_ctl(epollch_.getFd(), EPOLL_CTL_MOD, channel->getFd(), &ev) == -1)
    {
        perror("epoll_ctl: EPOLL_CTL_MOD");
        LOG_ERROR<<"errno:"<<errno;
    }
}

void
EPoll::delChannel(Channel *channel)
{
    if (::epoll_ctl(epollch_.getFd(), EPOLL_CTL_DEL, channel->getFd(), NULL) == -1)
    {
        perror("epoll_ctl: EPOLL_CTL_DEL");
        LOG_ERROR<<"errno:"<<errno;
    }
}

void
EPoll::poll(channel_vec &vec)
{
    int nfds = ::epoll_wait(epollch_.getFd(), events_.data(), static_cast<int>(events_.size()), -1);
    if (nfds == -1)
    {
        perror("epoll_wait");
        LOG_ERROR<<"errno:"<<errno;
    }
    LOG_TRACE<<"epoll channels available:"<<nfds;
    for(int i=0; i<nfds; i++)
    {
        struct epoll_event& ev = events_[i];
        Channel* channel = static_cast<Channel*>(ev.data.ptr);
        channel->setEvent(ev.events);
        vec.push_back( channel );
        LOG_TRACE<<"event:"<<static_cast<int>(ev.events);
    }
}


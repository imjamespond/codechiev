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

using namespace codechiev::net;

#define MAX_EVENTS 10
EPoll::EPoll():epollch_(::epoll_create1(EPOLL_CLOEXEC)),
events_(MAX_EVENTS)
{
    if (epollch_.getFd() == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
}

void
EPoll::addEvent(codechiev::net::Channel &channel)
{
    struct epoll_event ev;
    ev.events = EPOLLIN|EPOLLOUT;
#undef UseEpollET
#ifdef UseEpollET
    ev.events |= EPOLLET;
    LOG_DEBUG<<"UseEpollET";
#endif
    ev.data.fd = epollch_.getFd();
    ev.data.ptr = &channel;

    if(events_.size()==MAX_EVENTS)
    {
        events_.resize(events_.size()<<1);//double size
    }
    if (::epoll_ctl(epollch_.getFd(), EPOLL_CTL_ADD, channel.getFd(), &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }
}

void
EPoll::setEvent(codechiev::net::Channel &channel)
{
    struct epoll_event ev;
    ev.events = channel.getEvent();

    ev.data.fd = epollch_.getFd();
    ev.data.ptr = &channel;

    if (::epoll_ctl(epollch_.getFd(), EPOLL_CTL_MOD, channel.getFd(), &ev) == -1) {
        perror("epoll_ctl: EPOLL_CTL_MOD");
        exit(EXIT_FAILURE);
    }
}

void
EPoll::wait(Channel::chanenl_vec &vec)
{
    int nfds = ::epoll_wait(epollch_.getFd(), events_.data(), static_cast<int>(events_.size()), -1);
    if (nfds == -1) {
        perror("epoll_wait");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<nfds; i++)
    {
        struct epoll_event& ev = events_[i];
        vec.push_back( static_cast<Channel*>(ev.data.ptr) );
    }
}


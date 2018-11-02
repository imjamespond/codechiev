#include <stdio.h>
#include <stdlib.h>

#include "Epoll.hpp"
#include "sock_address.h"
#include <base/Logger.hpp>

using namespace codechiev::net;

Epoll::Epoll() : epChannel(0) {
  int epollfd = ::epoll_create1(0);
  if (epollfd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  epChannel.setFd(epollfd);
}

void Epoll::ctl(const ChannelPtr &_listenChannelPtr) {
  listenChannelPtr = _listenChannelPtr;

  ev.events = EPOLLIN;
  ev.data.fd = listenChannelPtr->getFd();
  if (::epoll_ctl(epChannel.getFd(), EPOLL_CTL_ADD, listenChannelPtr->getFd(),
                &ev) == -1) {
    perror("epoll_ctl: listen_sock");
    exit(EXIT_FAILURE);
  }
}

void Epoll::wait() {
  int nfds = ::epoll_wait(epChannel.getFd(), events, MAX_EVENTS, -1);
  if (nfds == -1) {
    perror("epoll_wait");
    exit(EXIT_FAILURE);
  }

  for (int n = 0; n < nfds; ++n) {
    if (events[n].data.fd == listenChannelPtr->getFd()) {
      LOG_DEBUG << "connect fd " << events[n].data.fd;

      sock_address_in addr;
      socklen_type addrlen;
      int conn_sock =
          accept(listenChannelPtr->getFd(), (struct sockaddr *)&addr, &addrlen);
      if (conn_sock == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
      }
      // setnonblocking(conn_sock);
      ::fcntl(conn_sock, F_SETFD, O_CLOEXEC | O_NONBLOCK);

      ev.events = EPOLLIN | EPOLLET;
      ev.data.fd = conn_sock;
      if (epoll_ctl(epChannel.getFd(), EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
        perror("epoll_ctl: conn_sock");
        exit(EXIT_FAILURE);
      }
    } else {
      // do_use_fd(events[n].data.fd);
      LOG_DEBUG << "active fd " << events[n].data.fd;
    }
  }
}
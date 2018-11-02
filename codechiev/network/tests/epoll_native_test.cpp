#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

#include <network/Channel.hpp>
#include <network/Epoll.hpp>
#include <network/sock_address.h>

#include <base/Logger.hpp>

using namespace codechiev::net;
using namespace codechiev::base;

int main() { 

  sock_address_in addr;
  socklen_type addrlen;
  set_sock_address(12345, addr);

#define MAX_EVENTS 10
  struct epoll_event ev, events[MAX_EVENTS];
  int listen_sock, conn_sock, nfds, epollfd;

  /* Code to set up listening socket, 'listen_sock',
    (socket(), bind(), listen()) omitted */
  listen_sock =
      ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
  if (listen_sock == -1)
    perror("socket");
  if (::bind(listen_sock, (sock_address *)&addr, sizeof(sock_address)) == -1)
    perror("bind");
#define LISTEN_BACKLOG 8192
  if (::listen(listen_sock, LISTEN_BACKLOG) == -1)
    perror("listen");

  epollfd = epoll_create1(0);
  if (epollfd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  ev.events = EPOLLIN;
  ev.data.fd = listen_sock;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
    perror("epoll_ctl: listen_sock");
    exit(EXIT_FAILURE);
  }

  for (;;) {
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }

    for (int n = 0; n < nfds; ++n) {
      if (events[n].data.fd == listen_sock) {
        LOG_DEBUG << "connect fd " << events[n].data.fd;

        conn_sock = accept(listen_sock, (struct sockaddr *)&addr, &addrlen);
        if (conn_sock == -1) {
          perror("accept");
          exit(EXIT_FAILURE);
        }
        // setnonblocking(conn_sock);
        ::fcntl(conn_sock, F_SETFD, O_CLOEXEC | O_NONBLOCK);

        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = conn_sock;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
          perror("epoll_ctl: conn_sock");
          exit(EXIT_FAILURE);
        }
      } else {
        // do_use_fd(events[n].data.fd);
        LOG_DEBUG << "active fd " << events[n].data.fd;
      }
    }
  }
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

#include <network/Channel.hpp>
#include <network/Eventloop.hpp>
#include <network/sock_address.h>

#include <base/Logger.hpp>

using namespace codechiev::net;
using namespace codechiev::base;

int main() { 

  sock_address_in addr;
  set_sock_address(12345, addr);

  int listen_sock;

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

  Epoll::ChannelPtr listenChannelPtr(new Channel(listen_sock));
  Epoll epoll;

  epoll.ctl(listenChannelPtr);

  Eventloop evLoop;
  evLoop.loop(epoll);
}
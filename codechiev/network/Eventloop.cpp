#include "Eventloop.hpp"

using namespace codechiev::net;

void Eventloop::loop(Epoll &epoll) {
  for (;;) {
    epoll.wait();
  }
}
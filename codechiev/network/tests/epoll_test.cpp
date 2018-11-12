#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <sys/epoll.h>
// #include <signal.h>
#include <boost/bind.hpp>

#include <network/TcpServer.hpp>

#include <base/Time.hpp>
#include <base/Logger.hpp>
// #include <base/Keyboard.hpp>

using namespace codechiev::net;
using namespace codechiev::base;

void onConnect(Channel *);
void onRead(Channel *, const char *, int);
void onWrite(Channel *, const char *, int);
void onClose(Channel *);
void epollHandler(Epoll *, Channel *, Channel *);

TcpServer endpoint;

int main()
{
  // struct sigaction st[] = {SIG_IGN};
  // sigaction(SIGPIPE, st, NULL);

  endpoint.setOnConnectFunc(boost::bind(&onConnect, _1));
  endpoint.setOnReadFunc(boost::bind(&onRead, _1, _2, _3));
  endpoint.setOnWriteFunc(boost::bind(&onWrite, _1, _2, _3));
  endpoint.setOnCloseFunc(boost::bind(&onClose, _1));
  endpoint.start(12345);

}

void onConnect(Channel *channel)
{
  LOG_DEBUG << "connect fd: " << channel->getFd();
}
void onRead(Channel *channel, const char *buf, int len)
{
  LOG_DEBUG << "read fd: " << channel->getFd()
            << ", buf: "<<buf
            << ", len: "<<len;
  // Must not shut down when channel is writable, otherwise will received SIGPIPE, broken pipe?
  // https://stackoverflow.com/questions/18935446/program-received-signal-sigpipe-broken-pipe
  if (strcmp(buf, "close\n") == 0)
  {
    channel->shutdown();
  }
  endpoint.send(channel, buf, len);
}
void onWrite(Channel *channel, const char *msg, int len)
{
  LOG_DEBUG << "write fd: " << channel->getFd();
}
void onClose(Channel *channel)
{
  LOG_DEBUG << "close fd: " << channel->getFd();
}
#ifndef Endpoint_hpp
#define Endpoint_hpp

#include <boost/function.hpp>
#include <queue>
#include <string>
#include <thread/mutex.hpp>
#include "epoll.hpp"
#include "loop.hpp"

namespace learn_cpp
{
  class Endpoint
  {
  public:
    typedef EventLoop<Epoll> Loop;
    typedef boost::function<void(const ChannelPtr &)> OnConnectFunc;
    typedef boost::function<void(const ChannelPtr &, const char *, int)> OnWriteFunc;
    typedef boost::function<void(const ChannelPtr &, const char *, int)> OnReadFunc;
    typedef boost::function<void(const ChannelPtr &)> OnReadEndFunc;
    typedef boost::function<void(const ChannelPtr &)> OnWrittenFunc;
    typedef boost::function<void(const ChannelPtr &)> OnCloseFunc;

    OnConnectFunc onConnect;
    OnCloseFunc onClose;
    OnReadFunc onRead;
    OnWriteFunc onWrite;
    OnReadEndFunc onReadEnd;
    OnWrittenFunc onWritten;

    Endpoint(Loop *);

    void Write(const ChannelPtr &, const std::string &);

  protected:
    void handleRead(const ChannelPtr &);
    void handleWrite(const ChannelPtr &);

    std::queue<std::string> queue;
    Mutex mutex;
    Loop *loop;
  };
}

#endif
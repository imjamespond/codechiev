#ifndef TcpServer_hpp
#define TcpServer_hpp

#include <boost/noncopyable.hpp>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <libev/SockAddress.hpp>

namespace codechiev
{
namespace libev
{

class TcpServer : public boost::noncopyable
{
  public:
    explicit TcpServer(int);
    ~TcpServer();

    void start();
  
  public:

  private:
    struct event_base *base;
    struct evconnlistener *listener;

    SockAddress addr;
};

} // namespace libev
} // namespace codechiev

#endif /* TcpServer_hpp */

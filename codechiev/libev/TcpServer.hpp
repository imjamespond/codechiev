#ifndef TcpServer_hpp
#define TcpServer_hpp

#include <boost/noncopyable.hpp>
#include <boost/unordered_map.hpp>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>

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
    void stop();
    void write(int fd, const char*);
    void broadcast(const char*);

  public:
    struct event_base *base;
    struct evconnlistener *listener;

    SockAddress addr;

    typedef boost::unordered_map<int, bufferevent *> BuffereventMap;
    BuffereventMap bevMap;
};

} // namespace libev
} // namespace codechiev

#endif /* TcpServer_hpp */

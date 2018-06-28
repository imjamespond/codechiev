#ifndef TcpEndpoint_hpp
#define TcpEndpoint_hpp

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>

#include <libev/SockAddress.h>

namespace codechiev
{
namespace libev
{

class TcpEndpoint : public boost::noncopyable
{
  public:
    explicit TcpEndpoint();
    virtual ~TcpEndpoint();

    typedef struct bufferevent bufferevent_struct;
    void write(bufferevent_struct *, const char *, size_t);

  public:
    struct event_base *base;

    typedef boost::function<int(bufferevent_struct *)> on_connect_fn;
    typedef boost::function<int(bufferevent_struct *, void *, int)> on_read_fn;
    typedef boost::function<int(bufferevent_struct *)> on_write_fn;
    typedef boost::function<int(bufferevent_struct *)> on_close_fn;

    on_connect_fn onConnect;
    on_read_fn onRead;
    on_write_fn onWrite;
    on_close_fn onClose;
};

void writecb(struct bufferevent *, void *);
void readcb(struct bufferevent *, void *);
void eventcb(struct bufferevent *, short, void *);

} // namespace libev
} // namespace codechiev

#endif /* TcpEndpoint_hpp */

#ifndef TcpEndpoint_hpp
#define TcpEndpoint_hpp

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

#include <event2/util.h> 
#include <event2/thread.h>

#include <libev/SockAddress.h>
#include <libev/Channel.hpp>

namespace codechiev
{
namespace libev
{

class TcpEndpoint : public boost::noncopyable
{
  public:
    explicit TcpEndpoint();
    virtual ~TcpEndpoint();

    static void Write(Channel::bufev_struct *, const char *, size_t);

  public:
    struct event_base *base;

    typedef boost::function<int(Channel::bufev_struct *)> on_connect_fn;
    typedef boost::function<int(Channel::bufev_struct *, void *, int)> on_read_fn;
    typedef boost::function<int(Channel::bufev_struct *)> on_write_fn;
    typedef boost::function<int(Channel::bufev_struct *)> on_close_fn;

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

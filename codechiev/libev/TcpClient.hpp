#ifndef TcpClient_hpp
#define TcpClient_hpp

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

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

class TcpClient : public boost::noncopyable
{
  public:
    explicit TcpClient(const char*);
    ~TcpClient();

    void connect();
    int stop();

    typedef struct bufferevent bufferevent_struct;
    void write(bufferevent_struct *, const char *);

    typedef boost::function<int(bufferevent_struct *)> on_connect_fn;
    typedef boost::function<int(bufferevent_struct *, void *, int)> on_read_fn;
    typedef boost::function<int(bufferevent_struct *)> on_write_fn;
    typedef boost::function<int(bufferevent_struct *)> on_close_fn;
    on_connect_fn onConnect;
    on_read_fn onRead;
    on_write_fn onWrite;
    on_close_fn onClose;

  public:
    struct event_base *base;

    struct sockaddr_storage connect_to_addr;
    int connect_to_addrlen;
    //SockAddress addr;
};

} // namespace libev
} // namespace codechiev

#endif /* TcpClient_hpp */

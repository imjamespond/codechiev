#ifndef TcpServer_hpp
#define TcpServer_hpp

#include <libev/SockAddress.h>
#include <libev/TcpEndpoint.hpp>

#include <event2/listener.h>

namespace codechiev
{
namespace libev
{

class TcpServer : public TcpEndpoint
{
  public:
    explicit TcpServer();
    explicit TcpServer(int);
    ~TcpServer();

    void start();
    void bind();
    int stop();


  public:
    struct event *listenev;
    struct evconnlistener *listener;
    sock_address addr;

    typedef boost::function<int(Channel::bufev_struct *)> on_accept_fn;
    on_accept_fn onAccept;
};

} // namespace libev
} // namespace codechiev

#endif /* TcpServer_hpp */

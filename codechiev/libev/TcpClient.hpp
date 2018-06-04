#ifndef TcpClient_hpp
#define TcpClient_hpp

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

class TcpClient : public boost::noncopyable
{
  public:
    explicit TcpClient(const char*);
    ~TcpClient();

    void connect();

  public:

  private:
    struct event_base *base;

    struct sockaddr_storage connect_to_addr;
    int connect_to_addrlen;
    //SockAddress addr;
};

} // namespace libev
} // namespace codechiev

#endif /* TcpClient_hpp */

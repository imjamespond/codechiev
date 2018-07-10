#ifndef TcpServer_hpp
#define TcpServer_hpp

#include <libev/SockAddress.h>
#include <libev/Channel.hpp>
#include <libev/TcpEndpoint.hpp>
#include <base/Thread.hpp>

#include <vector>
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

    typedef boost::function<int(Channel *)> on_accept_fn;
    on_accept_fn onAccept;

    class Worker
    {
      public:
        Worker(TcpServer *, int);
        ~Worker();
        void start();

        int connfd;
        TcpServer* server;
        struct event_base *base;
        struct event acceptev;
        codechiev::base::Thread::thread_ptr_t thread_;
    };

    typedef boost::shared_ptr<Worker> worker_ptr;
    typedef std::vector<worker_ptr> workers_vec;
    workers_vec workers;
};

} // namespace libev
} // namespace codechiev

#endif /* TcpServer_hpp */

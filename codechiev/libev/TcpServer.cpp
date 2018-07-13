#include "TcpServer.hpp"
#include "TcpServerHelper.hpp"

#include <base/Error.hpp>
#include <base/Logger.hpp>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

using namespace codechiev::libev;
using namespace codechiev::base;

TcpServer::TcpServer(int port) : TcpEndpoint(), listenev(NULL), listener(NULL), latch((int)worker_num)
{
  set_sock_address(port, addr);

  base = event_base_new();
  if (!base)
  {
    fprintf(stderr, "Could not initialize libevent!\n");
    throw 1;
  }

  for (int i(0); i < worker_num; i++)
  {
    worker_ptr wptr(new Worker(this, i));
    workers.push_back(wptr);
  }

  latch.latch();
}

TcpServer::~TcpServer()
{
  LOG_DEBUG;
  if (listener)
  {
    evconnlistener_free(listener);
  }
  if (listenev)
  {
    event_free(listenev);
  }
}

// void TcpServer::start()
// {
//   listener = evconnlistener_new_bind(base,
//                                      listener_cb,
//                                      (void *)this,
//                                      LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE | LEV_OPT_THREADSAFE,
//                                      -1,
//                                      (struct sockaddr *)&(addr),
//                                      sizeof(addr));

//   if (!listener)
//   {
//     fprintf(stderr, "Could not create a listener!\n");
//     throw 1;
//   }

//   event_base_dispatch(base);
// }

void TcpServer::bind()
{
  evutil_socket_t sockfd = ::socket(AF_INET,                                    //SOCK_NONBLOCK
                                    SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, //bitwise value
                                    0);                                         //protocol can be specified as 0
  if (sockfd == -1)
  {
    perror("socket");
    exit(1);
  }
  evutil_make_socket_nonblocking(sockfd);
  if (::bind(sockfd, (struct sockaddr *)&(addr), sizeof(addr)) < 0)
  {
    perror("bind");
    exit(1);
  }
  if (-1 == ::listen(sockfd, 1024))
  {
    perror("listen error");
    exit(EXIT_FAILURE);
  }
  listenev = event_new(base,
                       sockfd,
                       EV_READ | EV_PERSIST /*| EV_ET will probally cause connection failed*/,
                       accept_cb, this);
  event_add(listenev, NULL);
  event_base_dispatch(base);
}

int TcpServer::stop()
{
  return event_base_loopexit(base, NULL);
}

TcpServer::TcpServer()
{
  throw Error("default constructor is unavailible.");
}

// Worker

TcpServer::Worker::Worker(TcpServer *server, int i) : server(server)
{
  Thread::thread_func_t func = boost::bind(&TcpServer::Worker::start, this);
  std::string name("tcpserver-worker-");
  name += boost::lexical_cast<std::string>(i);
  thread_ = Thread::thread_ptr_t(new Thread(name, func));
  thread_->start();
}
TcpServer::Worker::~Worker()
{
  thread_->join();
}
void TcpServer::Worker::start()
{
  LOG_TRACE << "worker start";
  base = event_base_new();
  if (!base)
  {
    fprintf(stderr, "Could not initialize libevent!\n");
    throw 1;
  }

  int pipefd[2]; //a special instance of a file that has no contents on the filesystem
  if (::pipe(pipefd) == -1)
  {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  readfd = pipefd[0];
  writefd = pipefd[1];

  evutil_make_socket_nonblocking(readfd);
  evutil_make_socket_nonblocking(writefd);
  acceptev = event_new(base,
                       readfd,
                       EV_READ | EV_PERSIST,
                       worker_accept_cb, this);
  event_add(acceptev, NULL);

  server->latch.reduce();
  event_base_dispatch(base);
}
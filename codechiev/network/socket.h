#ifndef SockAddress_h
#define SockAddress_h

#include <errno.h>
#include <stdlib.h>
#include <stdio.h> //perror

#include <string.h> //memset

#ifndef _WIN32
/* For sockaddr_in */
#include <netinet/in.h>
#ifdef _XOPEN_SOURCE_EXTENDED
#include <arpa/inet.h>
#endif
/* For socket functions */
#include <sys/socket.h>
#endif

/* For gethostbyname */
#include <netdb.h>

namespace codechiev {
namespace net {

typedef struct sockaddr sock_address;
typedef struct sockaddr_in sock_address_in;
typedef socklen_t socklen_type;

inline void set_sock_address(sock_address_in &sin, int port, const char *host = NULL)
{
  ::memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET; // IPv4 Internet protocols
  sin.sin_port = ::htons(port);
  if (host)
    sin.sin_addr.s_addr = ::inet_addr(host);
  else
    sin.sin_addr.s_addr = INADDR_ANY;
}

inline int getSockfd()
{
  int sockfd =
      ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
  if (sockfd == -1)
    perror("socket");
  return sockfd;
}

inline int setReuseAddr(int sockfd)
{
  int on = 1;
  if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
  {
    perror("setsockopt");
    return -1;
  }
  return 0;
}

inline int Listen(int port, const char *host = NULL)
{

  sock_address_in addr;
  set_sock_address(addr, port, host);

  int listenfd = getSockfd();
  if (listenfd == -1)
    return -1;

  if (setReuseAddr(listenfd) == -1)
    return -1;

  if (::bind(listenfd, (sock_address *)&addr, sizeof(sock_address)) == -1)
    perror("bind");
#define LISTEN_BACKLOG 8192
  if (::listen(listenfd, LISTEN_BACKLOG) == -1)
    perror("listen");

  return listenfd;
}

inline int Accept(int fd)
{
  sock_address_in addr;
  socklen_type addrlen;
  
  int conn_sock = ::accept(fd, (struct sockaddr *)&addr, &addrlen);
  if (conn_sock == -1)
  {
    perror("accept");
    // exit(EXIT_FAILURE); 
  }

  return conn_sock;
}

inline int Connect(int port, const char *host)
{
  sock_address_in addr;
  set_sock_address(addr, port, host);

  int sockfd = getSockfd();
  if (sockfd == -1)
    return -1;

  if (setReuseAddr(sockfd) == -1)
    return -1;

  int res = ::connect(sockfd, (struct sockaddr *)&addr, sizeof(sock_address));
  if (res == -1)
  {
    if (EINPROGRESS != errno)
    {
      perror("connect");
    }
  }
  else if (res == 0)
  {
    printf("connection has succeeded immediately %d\n", sockfd);
  }

  return sockfd;
}

} // namespace net
} // namespace codechiev

#endif /* SockAddress_h */

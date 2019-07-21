#ifndef SockAddress_h
#define SockAddress_h

#include <errno.h>
#include <stdlib.h>
#include <stdio.h> //perror

#include <string.h> //memset

#ifndef _WIN32
/* For sockaddr_in */
#include <netinet/in.h>
#include <netinet/tcp.h>
#ifdef _XOPEN_SOURCE_EXTENDED
#include <arpa/inet.h>
#endif
/* For socket functions */
#include <sys/socket.h>
#endif

/* For gethostbyname */
#include <netdb.h>

namespace codechiev
{
namespace net
{

typedef struct sockaddr sock_address;
typedef struct sockaddr_in sock_address_in;
typedef socklen_t socklen_type;

inline void set_sock_address(sock_address_in &sin, int port, const char *host = NULL)
{
  ::memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;   // IPv4 Internet protocols
  sin.sin_port = htons(port); // ::htons(port); not OK with -O2 because it is being replaced with ::(__extension__ ({ register unsigned int __v, ...
  if (host)
    sin.sin_addr.s_addr = ::inet_addr(host);
  else
    sin.sin_addr.s_addr = INADDR_ANY;
}

inline int GetSockfd()
{
  int sockfd =
      ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
  if (sockfd == -1)
    perror("socket");
  return sockfd;
}

inline int SetReuseAddr(int sockfd)
{
  int optval = 1;
  if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
  {
    perror("setsockopt");
    return -1;
  }
  return 0;
}

inline int SetTcpNoDelay(int sockfd) //TCP_QUICKACK
{
  int optval = 1;
  if (::setsockopt(sockfd, SOL_TCP, TCP_NODELAY, &optval, sizeof(optval)) == -1)
  {
    perror("setsockopt");
    return -1;
  }
  return 0;
}

inline int SetKeepAlive(int sockfd)
{
  int optval = 1;
  if (::setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) == -1)
  {
    perror("setsockopt");
    return -1;
  }
  return 0;
}

inline void printSockopt(int sockfd)
{
  printf("printSockopt: %d", sockfd);
  int optval = 1;
  socklen_t optlen = sizeof(optval);
  if (::getsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) == 0)
  {
    printf(", SO_KEEPALIVE");
  }
  if (::getsockopt(sockfd, SOL_TCP, TCP_NODELAY, &optval, &optlen) == 0)
  {
    printf(", TCP_NODELAY");
  }
  if (::getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen) == 0)
  {
    printf(", SO_REUSEADDR");
  }
  int flags = ::fcntl(sockfd, F_GETFL, 0);
  if (flags & O_NONBLOCK)
  {
    printf(", O_NONBLOCK");
  }
  printf("\n");
}

inline int Listen(int port, const char *host = NULL)
{

  sock_address_in addr;
  set_sock_address(addr, port, host);

  int listenfd = GetSockfd();
  if (listenfd == -1)
    return -1;

  if (SetReuseAddr(listenfd) == -1)
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
  socklen_type addrlen = sizeof(addr);

  int conn_sock = ::accept(fd, (struct sockaddr *)&addr, &addrlen);
  if (conn_sock == -1)
  {
    perror("accept");
    // exit(EXIT_FAILURE);
  }

  return conn_sock;
}

inline int Connect(const char *port = "80", const char *host = "localhost")
{
  // struct addrinfo hints;
  // struct addrinfo *result, *rp;
  // int sockfd, s;

  // // if (argc < 3)
  // // {
  // //   fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
  // //   exit(EXIT_FAILURE);
  // // }

  // /* Obtain address(es) matching host/port */

  // memset(&hints, 0, sizeof(struct addrinfo));
  // hints.ai_family = AF_INET;       //AF_UNSPEC; /* Allow IPv4 or IPv6 */
  // hints.ai_socktype = SOCK_STREAM; //SOCK_DGRAM; /* Datagram socket */
  // hints.ai_flags = 0;
  // hints.ai_protocol = IPPROTO_TCP; /* Any protocol */

  // s = ::getaddrinfo(host, port, &hints, &result);
  // if (s != 0)
  // {
  //   fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
  //   exit(EXIT_FAILURE);
  // }

  // /* getaddrinfo() returns a list of address structures.
  //             Try each address until we successfully connect(2).
  //             If socket(2) (or connect(2)) fails, we (close the socket
  //             and) try the next address. */

  // for (rp = result; rp != NULL; rp = rp->ai_next)
  // {
  //   char host[256];
  //   getnameinfo(rp->ai_addr, rp->ai_addrlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST);
  //   // printf("host: %s, port: %d\n", host, ntohs(((struct sockaddr_in *)rp->ai_addr)->sin_port));

  //   sockfd = ::socket(rp->ai_family, rp->ai_socktype , rp->ai_protocol);
  //   if (sockfd == -1)
  //     continue;

  //   if (::connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1)
  //     break; /* Success */

  //   perror("connect");

  //   ::close(sockfd);
  // }

  // if (rp == NULL)
  // { /* No address succeeded */
  //   fprintf(stderr, "Could not connect\n");
  //   exit(EXIT_FAILURE);
  // }

  // ::freeaddrinfo(result); /* No longer needed */

  sock_address_in addr;
  set_sock_address(addr, ::atoi(port), host);

  int sockfd = GetSockfd();
  if (sockfd == -1)
    return -1;

  if (SetReuseAddr(sockfd) == -1)
    return -1;

  int res = ::connect(sockfd, (struct sockaddr *)&addr, sizeof(sock_address));
  if (res == -1)
  {
    if (EINPROGRESS != errno)
    {
      perror("connect");
    }
  }

  return sockfd;
}

} // namespace net
} // namespace codechiev

#endif /* SockAddress_h */

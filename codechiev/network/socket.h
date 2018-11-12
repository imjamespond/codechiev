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

inline void set_sock_address(int port, sock_address_in &sin) {
  ::memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET; // IPv4 Internet protocols
  sin.sin_port = ::htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
}

inline int Listen(int port) {

  sock_address_in addr;
  set_sock_address(port, addr);

  int listen_sock =
      ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
  if (listen_sock == -1)
    perror("socket");

  int on = 1;
  if (::setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    perror("setsockopt");

  if (::bind(listen_sock, (sock_address *)&addr, sizeof(sock_address)) == -1)
    perror("bind");
#define LISTEN_BACKLOG 8192
  if (::listen(listen_sock, LISTEN_BACKLOG) == -1)
    perror("listen");

  return listen_sock;
}

inline int Accept(int fd)
{
  sock_address_in addr;
  socklen_type addrlen;
  int conn_sock = ::accept(fd, (struct sockaddr *)&addr, &addrlen);
  if (conn_sock == -1)
  {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  return conn_sock;
}

} // namespace net
} // namespace codechiev

#endif /* SockAddress_h */

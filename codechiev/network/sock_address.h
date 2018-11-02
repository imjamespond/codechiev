#ifndef SockAddress_h
#define SockAddress_h

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

namespace codechiev
{
namespace net
{

typedef struct sockaddr sock_address;
typedef struct sockaddr_in sock_address_in;
typedef socklen_t socklen_type;

inline void set_sock_address(int port, sock_address_in &sin)
{
  ::memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET; //IPv4 Internet protocols
  sin.sin_port = ::htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
} 

} // namespace libev
} // namespace codechiev

#endif /* SockAddress_h */

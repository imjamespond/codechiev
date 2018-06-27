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
namespace libev
{

typedef struct sockaddr_in sock_address;

void set_sock_address(int, sock_address &);

} // namespace libev
} // namespace codechiev

#endif /* SockAddress_h */

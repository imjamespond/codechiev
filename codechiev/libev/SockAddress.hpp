#ifndef SockAddress_h
#define SockAddress_h

#include <string.h> //memset

#ifndef _WIN32
#include <netinet/in.h>
#ifdef _XOPEN_SOURCE_EXTENDED
#include <arpa/inet.h>
#endif
#include <sys/socket.h>
#endif

namespace codechiev
{
namespace libev
{

struct SockAddress
{ 
    explicit SockAddress(int );
    ~SockAddress();

    struct sockaddr_in sin;
};

} // namespace libev
} // namespace codechiev

#endif /* SockAddress_h */

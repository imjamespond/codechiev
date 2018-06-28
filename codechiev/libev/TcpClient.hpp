#ifndef TcpClient_hpp
#define TcpClient_hpp

#include <libev/TcpEndpoint.hpp>

namespace codechiev
{
namespace libev
{

class TcpClient : public TcpEndpoint
{
  public:
    explicit TcpClient(const char*);
    ~TcpClient();

    void start(int = 0);
    int stop();

    void connect();

  public:
    struct sockaddr_storage connect_to_addr;
    int connect_to_addrlen; 

};

} // namespace libev
} // namespace codechiev

#endif /* TcpClient_hpp */

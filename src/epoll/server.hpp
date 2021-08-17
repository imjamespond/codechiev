#ifndef Server_hpp
#define Server_hpp

#include "endpoint.hpp"

namespace learn_cpp
{
  class Server : public Endpoint
  {
  private:
    Channel *chan;

    void handleAccept(const ChannelPtr &, int, Loop *);
    void handle(const ChannelPtr &, int);

  public:
    Server(Loop *);
    ~Server();
  };

}

#endif
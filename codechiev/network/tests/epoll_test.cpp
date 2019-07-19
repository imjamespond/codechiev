#include <network/Eventloop.hpp>
#include <network/Epoll.hpp>
#include <network/TcpServer.hpp>
#include <base/Logger.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

const char *port = "12345";
const char *host = "127.0.0.1";

int main(int num, const char **args)
{ 
  if (num > 1)
  {
    host = args[1];
  }
  if (num > 2)
  {
    port = args[2]; 
  }

  LOG_INFO << "host: " << host << ", port: " << port;

  TcpServer serv1(port, host, true);

  {
    Eventloop<Epoll> serv1Loop;
    serv1.start(&serv1Loop);
  }

}
#include "SockAddress.hpp"

using namespace codechiev::libev;

//tcp socket
SockAddress::SockAddress(int port)
{
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET; //tcp
  sin.sin_port = htons(port);
}

SockAddress::~SockAddress()
{
}
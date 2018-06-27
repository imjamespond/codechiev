#include "SockAddress.h"

//tcp socket
void 
codechiev::libev::set_sock_address(int port, sock_address &sin)
{
  ::memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET; //IPv4 Internet protocols
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
} 
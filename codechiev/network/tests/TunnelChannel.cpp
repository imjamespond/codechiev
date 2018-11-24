#include "TunnelChannel.hpp" 

#include <base/UUID.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TunnelChannel::TunnelChannel(int _sockfd) : Channel(this, _sockfd)
{
  session = UUID::GenUUID();
}
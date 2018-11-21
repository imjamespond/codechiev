#include "TunnelChannel.hpp" 

#include <base/UUID.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TunnelChannel::TunnelChannel(int _sockfd) : Channel(_sockfd)
{
  session = UUID::GenUUID();
}

TunnelChannel::TunnelChannel(int _sockfd, const UUID::uuid_t& _session) : Channel(_sockfd)
{
  session = _session;
}
#ifndef TunnelChannel_hpp
#define TunnelChannel_hpp

#include <network/Channel.hpp>
#include <base/UUID.hpp>

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace codechiev {
namespace net {

struct Tunnel;

class TunnelChannel : public Channel
{
public:
  typedef codechiev::base::UUID UUID;
  typedef boost::weak_ptr<Channel> TunnelPtr;

  explicit TunnelChannel(int);

  UUID::uuid_t session;
  TunnelPtr tunnel;
};



} // namespace net
} // namespace codechiev

#endif /* TunnelChannel_hpp */
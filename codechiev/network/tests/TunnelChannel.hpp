#ifndef TunnelChannel_hpp
#define TunnelChannel_hpp

#include <network/Channel.hpp>
#include <base/UUID.hpp>

#include <boost/weaked_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace codechiev {
namespace net {

struct Tunnel;

class TunnelChannel : public Channel
{
public:
  typedef codechiev::base::UUID UUID;
  typedef boost::weaked_ptr<struct Tunnel> UUID;

  explicit TunnelChannel(int, TunnelChannel*);

  Tunnel tunnel0;
};

struct Tunnel
{
  Tunnel();

  UUID::uuid_t session;
};

} // namespace net
} // namespace codechiev

#endif /* TunnelChannel_hpp */
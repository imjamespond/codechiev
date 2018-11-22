#ifndef TunnelChannel_hpp
#define TunnelChannel_hpp

#include <network/Channel.hpp>
#include <base/UUID.hpp>

namespace codechiev {
namespace net {
class TunnelChannel : public Channel
{
public:
  typedef  codechiev::base::UUID UUID;

  explicit TunnelChannel(int); 
  explicit TunnelChannel(int, const UUID::uuid_t&); 

  UUID::uuid_t session;
};
} // namespace net
} // namespace codechiev

#endif /* TunnelChannel_hpp */
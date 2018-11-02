#ifndef Eventloop_hpp
#define Eventloop_hpp

#include "Epoll.hpp"

namespace codechiev {
namespace net {
class Eventloop {
public:
  void loop(Epoll &);
};
} // namespace net
} // namespace codechiev

#endif /* Eventloop_hpp */
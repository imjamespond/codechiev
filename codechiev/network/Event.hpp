#ifndef Event_hpp
#define Event_hpp

#include "Epoll.hpp"
#include "Eventloop.hpp"
 
#include <deque>

namespace codechiev
{
namespace net
{
class Event
{
public:
  Event();
  ~Event();

private:
  Channel p1Channel;
  Channel p2Channel;
};
} // namespace net
} // namespace codechiev

#endif /* Event_hpp */
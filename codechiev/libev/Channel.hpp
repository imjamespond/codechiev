#ifndef Channel_hpp
#define Channel_hpp

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/buffer.h>

namespace codechiev
{
namespace libev
{

class Channel 
{
  public:
    explicit Channel();
    ~Channel();

    static void Decode(struct evbuffer *);

  private:
    int len_;
};

} // namespace libev
} // namespace codechiev

#endif /* Channel_hpp */

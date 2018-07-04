#ifndef Channel_hpp
#define Channel_hpp

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h> 

#include <vector>

namespace codechiev
{
namespace libev
{
  
class Channel 
{
  public:
    typedef struct bufferevent bufev_struct;

    explicit Channel(bufev_struct *);
    explicit Channel();
    ~Channel();

    int decode();
    static const char * Encode(const char *);

  private:
    int head_;
    int send_left_;

    typedef std::vector<char> buf_vec;
    buf_vec send_buf_;

    bufev_struct *bufev_;
};

} // namespace libev
} // namespace codechiev

#endif /* Channel_hpp */

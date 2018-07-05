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
    typedef std::vector<char> buf_vec;

    explicit Channel(bufev_struct *);
    explicit Channel();
    ~Channel();

    int decode(const char *, int );
    const char *encode(const char *);
    void send(const char *);
    inline int sendBufSize() { return send_size_;}
    inline const buf_vec &sendBuf() { return send_buf_; }

  private:
    int head_;

    int send_cursor_;
    int send_size_;

    buf_vec send_buf_; 
    bufev_struct *bufev_;
};

} // namespace libev
} // namespace codechiev

#endif /* Channel_hpp */

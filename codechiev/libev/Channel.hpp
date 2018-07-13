#ifndef Channel_hpp
#define Channel_hpp

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h> 

#include <vector>
#include <boost/function.hpp> 

namespace codechiev
{
namespace libev
{
  struct SendBuffer
  { 
    typedef std::vector<char> buf_vec;
    buf_vec send_buf;
    int send_size;
    SendBuffer() : send_buf(1 << 10), send_size(0)
    {}
  };

  class TcpEndpoint;
  class Channel 
  {
    public: 
      typedef boost::function<void(const char*, int, Channel *)> on_message_func;//msg, head, fd
      typedef struct bufferevent bufev_struct;

      explicit Channel(TcpEndpoint *, bufev_struct *);
      explicit Channel();
      ~Channel();

      int decode(const char *, int );
      const char *encode(const char *, SendBuffer *);
      void send(const char *);
      // inline int getSendBufSize() { return send_size_;}
      // inline void setSendBufSize(int size) { send_size_ = size;}
      // inline const buf_vec &sendBuf() { return send_buf_; }

      TcpEndpoint * const endpoint;
      bufev_struct * const bufev;
      on_message_func onMessage;
      int fd;

      Channel *next;
      Channel *prev;
    private:
      int head_; 
      // int send_cursor_;
      // int send_size_;
      // buf_vec send_buf_;// poor performance!!!
  };

} // namespace libev
} // namespace codechiev

#endif /* Channel_hpp */

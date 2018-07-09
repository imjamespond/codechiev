#include "Channel.hpp"
#include "TcpEndpoint.hpp"

#include <base/Logger.hpp>
#include <base/Singleton.hpp>

#include <string>

using namespace codechiev::base;
using namespace codechiev::libev;

const int __head_len__(sizeof(int));
const int __max_len__(1<<16);

Channel::Channel(TcpEndpoint *endpoint, bufev_struct *bufev) : endpoint(endpoint),
                                          bufev(bufev),
                                          head_(-1)
                                          // send_cursor_(0),
                                          // send_size_(0)
                                          // send_buf_(__max_len__)
{
  fd = bufferevent_getfd(bufev);//evutil_socket_t
}

Channel::~Channel()
{
  STREAM_TRACE<<fd;
  bufferevent_free(bufev);
}

int Channel::decode(const char *data, int len)
{ 
  int read(0);
  STREAM_TRACE << " len:" << len;

  for(;;)
  {

    //read 4 bytes
    if ((len >= __head_len__) && (head_ < 0))
    { 
      ::memcpy(&head_, data+read, sizeof head_); 
      read += __head_len__;
      len -= __head_len__;

      STREAM_TRACE << " head:" << head_;
    }

    //one complete msg
    if (head_ > 0 && len >= head_)
    { 
      const char * msg = data+read;
      STREAM_TRACE << " msg head:" << head_ ;
      if(onMessage) 
      {
        onMessage(msg, head_, this);
      } 
      
      read += head_;
      len -= head_;
      head_ = -1;
    }
    STREAM_TRACE << " read:" << read << " left:" << len;
    //incomplete msg
    if (len < __head_len__)
    {
      return read;
    }
    if (head_ > 0 && len < head_)
    {
      return read;
    }
    //read all
    if (len == 0)
    {
      return read;
    } 
    //error head
    if ((__max_len__ < head_) || (head_ == 0))
    {
      return read;
    } 
  }
  return 0;
}

const char *
Channel::encode(const char *msg, SendBuffer *buf)
{
  int head(::strlen (msg));
  int head_len(sizeof(int));
  if(head_len>__max_len__)
  {
    return NULL;
  }
  ::memcpy(&buf->send_buf[buf->send_size], &head, head_len);
  buf->send_size += head_len;
  // send_cursor_+=head_len;
  std::copy(msg, msg + head, buf->send_buf.begin() + buf->send_size);
  buf->send_size += head;
  // send_cursor_+=head;
  return buf->send_buf.data();
}

void
Channel::send(const char *msg)
{
  SendBuffer *buf = ThreadSingleton<SendBuffer>::Get();
  const char *encoded = encode(msg, buf);
  if(encoded)
  {
    TcpEndpoint::Write(this->bufev, encoded, buf->send_size);
    buf->send_size = 0;
  }
}
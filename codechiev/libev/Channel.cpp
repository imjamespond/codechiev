#include "Channel.hpp"
#include <base/Logger.hpp>
#include <string>

using namespace codechiev::libev; 

const int __int_len__(sizeof(int));
const int __max_len__(1<<16);

Channel::Channel(Channel::bufev_struct *bufev) : head_(-1),
                                                 send_cursor_(0),
                                                 send_size_(0),
                                                 send_buf_(__max_len__),
                                                 bufev_(bufev)
{}

Channel::~Channel()
{}

int Channel::decode(const char *data, int len)
{ 
  int read(0);
  STREAM_TRACE << " len:" << len;

  for(;;)
  {

    //read 4 bytes
    if ((len >= __int_len__) && (head_ < 0))
    { 
      ::memcpy(&head_, data, sizeof head_); 
      read += __int_len__;
      len -= __int_len__;

      STREAM_TRACE << " head:" << head_;
    }
    //one complete msg
    if (head_ && (len >= head_))
    {
      read += head_;
      len -= head_;
  
      STREAM_TRACE << " msg:" << data+read << ", head:" << head_;
      head_ = -1;
    }

    if(head_ && (len < head_))
    {
      return read;
    }

    if ((__max_len__ < head_) || (head_ == 0))
    {
      // evbuffer_drain(evbuf, len);
      return len;
    } 
  }
  return 0;
}

const char *
Channel::encode(const char *msg)
{
  int head(::strlen (msg));
  int head_len(sizeof(int));
  ::memcpy(&send_buf_[send_cursor_], &head, head_len);
  std::copy(msg, msg + head, send_buf_.begin() + head_len);
  int size(head + head_len);
  send_size_+=size;
  send_cursor_+=size;
  return send_buf_.data();
}
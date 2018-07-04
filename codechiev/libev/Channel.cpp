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

int Channel::decode()
{
  struct evbuffer *evbuf = bufferevent_get_input(bufev_);
  int len = evbuffer_get_length(evbuf);
  STREAM_TRACE << " len:" << len;
  //read 4 bytes
  if (len >= __int_len__ && head_ < 0)
  {
    unsigned char * _len = evbuffer_pullup(evbuf, __int_len__);
    int * __len = reinterpret_cast<int *>(_len);
    head_ = *__len;
    len -= __int_len__;

    STREAM_TRACE << " head:" << head_;
    evbuffer_drain(evbuf, __int_len__);
  }
  //one complete msg
  if (head_ && len >= head_)
  {
    unsigned char *_msg = evbuffer_pullup(evbuf, head_);
    len -= head_;

    STREAM_TRACE << " msg:" << _msg;
    evbuffer_drain(evbuf, head_);
  }

  if (__max_len__ < head_ || head_ == 0)
  {
    evbuffer_drain(evbuf, len);
    return 0;
  }

  return len;
}

const char *
Channel::encode(const char *msg)
{
  int head(::strlen (msg));
  int head_len(sizeof(int));
  ::memcpy(&send_buf_[0], &head, head_len);
  std::copy(msg, msg + head, send_buf_.begin() + head_len);
  send_size_ = head + head_len;
  return send_buf_.data();
}
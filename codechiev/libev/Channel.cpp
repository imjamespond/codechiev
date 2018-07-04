#include "Channel.hpp"

#include <base/Logger.hpp>

using namespace codechiev::libev; 

const int __int_len__(sizeof(int));

Channel::Channel(Channel::bufev_struct *bufev) : head_(-1), bufev_(bufev)
{}

Channel::~Channel()
{}

int Channel::decode()
{
  struct evbuffer *evbuf = bufferevent_get_input(bufev_);
  int len = evbuffer_get_length(evbuf);
  STREAM_INFO<<"len:"<<len;
  //read 4 bytes
  if (len >= __int_len__ && head_ < 0)
  {
    unsigned char * _len = evbuffer_pullup(evbuf, __int_len__);
    int * __len = reinterpret_cast<int *>(_len);
    head_ = *__len;
    len -= __int_len__;

    STREAM_INFO << " head:" << head_;
    evbuffer_drain(evbuf, __int_len__);
  }
  //one complete msg
  if (head_ && len >= head_)
  {
    unsigned char *_msg = evbuffer_pullup(evbuf, head_);
    len -= head_;

    STREAM_INFO << " msg:" << _msg;
    evbuffer_drain(evbuf, head_);
  }

  return 0;
}

const char *
Channel::Encode(const char *)
{
  return NULL;
}
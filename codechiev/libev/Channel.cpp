#include "Channel.hpp"

#include <base/Logger.hpp>

using namespace codechiev::libev;

const int __int_len__(sizeof(int));

Channel::Channel():len_(0)
{}

Channel::~Channel(){}

void
Channel::Decode(struct evbuffer *evbuf)
{
  int len = evbuffer_get_length(evbuf);
  int head(-1);
  STREAM_INFO<<"len:"<<len;
  //read 4 bytes
  if(len>=__int_len__ && head<0)
  {
    unsigned char * _len = evbuffer_pullup(evbuf, __int_len__);
    int * __len = reinterpret_cast<int *>(_len);
    head = * __len;
    STREAM_INFO << " head:" << head;
    len -= __int_len__;

    evbuffer_drain(evbuf, __int_len__);
  }
  if (head && len>=head)
  {
    unsigned char *_msg = evbuffer_pullup(evbuf, head);
    STREAM_INFO << " msg:" << _msg;
    evbuffer_drain(evbuf, head);
  }
  // else if()
  // {

  // }
}

void 
Channel::Encode(struct evbuffer *evbuf, const char *msg, int len)
{
  
}
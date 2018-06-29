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
  STREAM_INFO<<"len:"<<len;
  //read 4 bytes
  if(len>=__int_len__)
  {
    unsigned char * _len = evbuffer_pullup(evbuf, __int_len__);
    int * __len = reinterpret_cast<int *>(_len);
    STREAM_INFO << * __len;
  }
  evbuffer_drain(evbuf, len);
  // else if()
  // {

  // }
}
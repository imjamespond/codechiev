#include <libev/TcpServer.hpp> 

#include <boost/bind.hpp>
#include <boost/unordered_map.hpp> 

#include "chat_room_server.hpp"

using namespace codechiev::base;
using namespace codechiev::libev;

void 
ChatRoomServer::onMessage(const char* msg, int len, Channel *channel)
{ 
    std::string msg_str(msg, len);
    ChatRoomServer * const serv = static_cast<ChatRoomServer *>(channel->endpoint); 
    queue.add(boost::bind(&ChatRoomServer::addCount, serv));
}

int onServAccept(Channel *channel)
{ 
    ChatRoomServer * const serv = static_cast<ChatRoomServer *>(channel->endpoint); 
    serv->channels[channel->fd] = ChatRoomServer::channel_ptr(channel);
    channel->onMessage = boost::bind(&ChatRoomServer::onMessage, serv, _1,_2,_3);

    STREAM_TRACE<<channel->fd;
    return 0;
}

int onServClose(Channel *channel)
{
    ChatRoomServer * const serv = static_cast<ChatRoomServer *>(channel->endpoint); 
    serv->channels.erase(channel->fd);
    STREAM_TRACE;
    return 0;
}

int onServRead(Channel *channel)
{
    struct evbuffer *evbuf = bufferevent_get_input(channel->bufev);
    int len = evbuffer_get_length(evbuf);
    unsigned char *data = evbuffer_pullup(evbuf, len);
    int has_read = channel->decode((const char *)data,len);//consider put it in multiple threads
    evbuffer_drain(evbuf, has_read);
    
    return 0;
}

int onServWrite(Channel *channel)
{
    // STREAM_INFO;
    return 0;
}

ChatRoomServer::ChatRoomServer() : TcpServer(12345), mutex_(new Mutex)
{ 
    queue.start();
}

void 
ChatRoomServer::broadcast(const char * msg)
{
  //TODO must lock buffer, lock bevMap
  ChannelMap::iterator it;
  for (it = channels.begin(); it != channels.end(); ++it)
  {
    channel_ptr channel = it->second;
    if (channel) 
    {
        channel->send(msg); 
    }
    else
    {
        LOG_TRACE << "bufferevent is null";
    }
    
  }
}

int 
ChatRoomServer::totalClient()
{
    MutexLock lock(mutex_);
    return channels.size();
}
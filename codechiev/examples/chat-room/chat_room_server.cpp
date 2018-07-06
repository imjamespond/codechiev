#include <libev/TcpServer.hpp>
#include <libev/Channel.hpp>
#include <libev/Signal.hpp>
#include <base/Logger.hpp>
#include <base/Mutex.hpp>

#include <boost/bind.hpp>
#include <boost/unordered_map.hpp> 

#include "chat_room_server.hpp"

using namespace codechiev::base;
using namespace codechiev::libev;

int onServAccept(Channel *channel)
{ 
    ChatRoomServer * const serv = static_cast<ChatRoomServer *>(channel->endpoint);
    evutil_socket_t fd = bufferevent_getfd(channel->bufev);
    serv->channels[fd] = channel;
    // STREAM_INFO;
    return 0;
}

int onServClose(Channel *channel)
{
    ChatRoomServer * const serv = static_cast<ChatRoomServer *>(channel->endpoint);
    evutil_socket_t fd = bufferevent_getfd(channel->bufev);
    serv->channels.erase(fd);
    // STREAM_INFO;
    return 0;
}

int onServRead(Channel *channel)
{
    // STREAM_INFO;
    // ChatRoomServer * const serv = static_cast<ChatRoomServer *>(channel->endpoint);
    // evutil_socket_t fd = bufferevent_getfd(channel->bufev);
    // ChatRoomServer::channel_ptr channel = server->clients[fd];

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
{ }

void 
ChatRoomServer::broadcast(const char * msg)
{
  //TODO must lock buffer, lock bevMap
  ChannelMap::iterator it;
  for (it = channels.begin(); it != channels.end(); ++it)
  {
    Channel *channel = it->second;
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
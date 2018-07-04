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

int onServAccept(TcpServer *server, Channel::bufev_struct *bev)
{
    ChatRoomServer *serv = static_cast<ChatRoomServer *>(server);
    evutil_socket_t fd = bufferevent_getfd(bev);
    serv->clients[fd] = ChatRoomServer::channel_ptr(new Channel(bev));
    // STREAM_INFO;
    return 0;
}

int onServClose(TcpEndpoint *endpoint, Channel::bufev_struct *bev)
{
    ChatRoomServer *server = static_cast<ChatRoomServer *>(endpoint);
    evutil_socket_t fd = bufferevent_getfd(bev);
    server->clients.erase(fd);
    // STREAM_INFO;
    return 0;
}

int onServRead(TcpEndpoint *endpoint, Channel::bufev_struct *bev, void *data, int len)
{
    // STREAM_INFO;
    ChatRoomServer *server = static_cast<ChatRoomServer *>(endpoint);
    evutil_socket_t fd = bufferevent_getfd(bev);
    ChatRoomServer::channel_ptr channel = server->clients[fd];

    // while (channel.decode()){}
    channel->decode();
    
    return 0;
}

int onServWrite(TcpEndpoint *endpoint, Channel::bufev_struct *bev)
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
  BuffereventMap::iterator it;
  for (it = clients.begin(); it != clients.end(); ++it)
  {
    channel_ptr channel = it->second;
    if (channel) 
    {
        // channel->send(msg, ::strlen(msg)); 
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
    return clients.size();
}
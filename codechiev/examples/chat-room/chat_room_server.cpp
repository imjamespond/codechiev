#include <libev/TcpServer.hpp>
#include <libev/Channel.hpp>
#include <libev/Signal.hpp>
#include <base/Logger.hpp>
#include <base/Mutex.hpp>

#include <boost/bind.hpp>
#include <boost/unordered_map.hpp> 

#include "chat_room_server.h"

using namespace codechiev::base;
using namespace codechiev::libev;

int onServAccept(TcpServer *serv, TcpServer::bufferevent_struct *bev)
{
    // ChatRoomServer *server = static_cast<ChatRoomServer *>(serv);
    STREAM_INFO;
    return 0;
}

int onServClose(TcpEndpoint *endpoint, TcpEndpoint::bufferevent_struct *bev)
{
    // ChatRoomServer *server = static_cast<ChatRoomServer *>(endpoint);
    STREAM_INFO;
    return 0;
}

int onServRead(TcpEndpoint *endpoint, TcpEndpoint::bufferevent_struct *bufev, void *data, int len)
{
    // std::string msg((char *)data, len);
    struct evbuffer *evbuf = bufferevent_get_input(bufev);
    Channel::Decode(evbuf);
    return 0;
}

int onServWrite(TcpEndpoint *endpoint, TcpEndpoint::bufferevent_struct *bev)
{
    STREAM_INFO;
    return 0;
}


ChatRoomServer::ChatRoomServer() : TcpServer(12345)
{
    mutexCli = Mutex::mutex_ptr(new Mutex);
}

void 
ChatRoomServer::broadcast(const char * msg)
{
  //TODO must lock buffer, lock bevMap
  BuffereventMap::iterator it;
  for (it = clients.begin(); it != clients.end(); ++it)
  {
    bufferevent_struct *bev = it->second;

    if (bev) {
        write(bev, msg, ::strlen(msg)); 
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
    MutexLock lock(mutexCli);
    return clients.size();
}
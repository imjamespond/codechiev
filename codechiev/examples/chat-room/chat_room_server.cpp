#include <libev/TcpServer.hpp>

#include <boost/bind.hpp>
#include <boost/unordered_map.hpp>

#include "chat_room_server.hpp"

using namespace codechiev::base;
using namespace codechiev::libev;

void ChatRoomServer::onMessage(const char *msg, int len, Channel *channel)
{
    std::string msg_str(msg, len);
    ChatRoomServer *const serv = static_cast<ChatRoomServer *>(channel->endpoint);
    queue.add(boost::bind(&ChatRoomServer::addCount, serv, len));
}

int onServAccept(Channel *channel)
{
    ChatRoomServer *const serv = static_cast<ChatRoomServer *>(channel->endpoint);
    // serv->channels[channel->fd] = ChatRoomServer::channel_ptr(channel);
    channel->onMessage = boost::bind(&ChatRoomServer::onMessage, serv, _1, _2, _3);

    {
        MutexLock lock(serv->mutex);
        if (serv->channel)
        {
            channel->prev = serv->channel;
            serv->channel->next = channel;
            serv->channel = channel;
        }
        else
        {
            serv->channel = channel;
        }
    }

    STREAM_TRACE << channel->fd;
    return 0;
}

int onServClose(Channel *channel)
{
    ChatRoomServer *const serv = static_cast<ChatRoomServer *>(channel->endpoint);
    // serv->channels.erase(channel->fd);

    {
        MutexLock lock(serv->mutex);
        if (channel->prev)
        {
            channel->prev->next = channel->next;
            if (channel == serv->channel)
            {
                serv->channel = channel->prev;
            }
        }
        else if (channel->next)
        {
            channel->next->prev = channel->prev;
            if (channel == serv->channel)
            {
                serv->channel = channel->next;
            }
        }
        else
        {
            serv->channel = NULL;
        }
        delete channel;
    }

    STREAM_TRACE;
    return 0;
}

int onServRead(Channel *channel)
{
    struct evbuffer *evbuf = bufferevent_get_input(channel->bufev);
    int len = evbuffer_get_length(evbuf);
    unsigned char *data = evbuffer_pullup(evbuf, len);
    int has_read = channel->decode((const char *)data, len); //consider put it in multiple threads
    evbuffer_drain(evbuf, has_read);

    return 0;
}

int onServWrite(Channel *channel)
{
    // STREAM_INFO;
    return 0;
}

ChatRoomServer::ChatRoomServer() : TcpServer(12345), mutex(new Mutex), recv_bytes(0)
{
    queue.start();
}

void ChatRoomServer::broadcast(const char *msg)
{
    //TODO must lock buffer, lock bevMap
    //   ChannelMap::iterator it;
    //   for (it = channels.begin(); it != channels.end(); ++it)
    //   {
    //     channel_ptr channel = it->second;
    //     if (channel)
    //     {
    //         channel->send(msg);
    //     }
    //     else
    //     {
    //         LOG_TRACE << "bufferevent is null";
    //     }

    //   }

    MutexLock lock(mutex);

    Channel *_channel = channel;
    if (_channel)
    {
        _channel->send(msg);
        while (_channel->next)
        {
            _channel->send(msg);
            _channel = _channel->next;
        }
        while (_channel->prev)
        {
            _channel->send(msg);
            _channel = _channel->prev;
        }
    }
}

int ChatRoomServer::totalClient()
{
    // return channels.size();
    MutexLock lock(mutex);

    int total(0);
    Channel *_channel = channel;
    if (_channel)
    {
        ++total;
        while (_channel->next)
        {
            ++total;
            _channel = _channel->next;
        }
        while (_channel->prev)
        {
            ++total;
            _channel = _channel->prev;
        }
    } 
    return total;
}
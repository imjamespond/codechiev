#include "TcpClient.hpp"
#include "socket.h"

#include <boost/bind.hpp>
#include <base/Logger.hpp>
#include <base/Time.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

TcpClient::TcpClient(bool edge_mode) : TcpEndpoint(edge_mode), loop(NULL)
{
}

void TcpClient::connect(const char *port, const char *host)
{
    assert(loop);
    int conn_sock = Connect(port, host);

    Channel *conn;
    if (createChannel)
    {
        conn = createChannel(conn_sock);
    }
    else
    {
        conn = Channel::CreateRaw(conn_sock);
    } 
    conn->loop = loop;
    conn->ptr = Channel::ChannelPtr(conn);

    loop->getPoll()->ctlAdd(conn, EPOLLOUT | EPOLLERR);
}

void TcpClient::connect(Channel *channel)
{
    channel->loop = loop;

    loop->getPoll()->ctlAdd(channel, EPOLLOUT | EPOLLERR);
}

void TcpClient::start(Loop *loop)
{
    this->loop = loop;
    Handler handler = boost::bind(&TcpClient::epoll_handler_, this, _1);
    loop->getPoll()->setHandler(handler);
    loop->loop();
}

void TcpClient::epoll_handler_(const Channel::ChannelPtr &channel)
{
    // LOG_DEBUG << "fd: " << channel->getFd() << ", events: " << channel->getEvents();

    if (channel->connected())
    {
        handle_event_(channel);
    }
    else if (channel->writable())
    {
        if (channel->check())
        {
            channel->setNonblocking();
            channel->setConnected();

            if (edge_mode)
            {
                loop->getPoll()->setReadable(channel.get());
            }
            else
            {
                loop->getPoll()->setReadable(channel.get(), EPOLLIN);
            }

            if (onConnect)
                onConnect(channel);
        }
        else
        {
            channel->setClosed();
        }
    }

    if (channel->closed())
    {
        if (onClose)
        {
            onClose(channel);
        }
        close_(loop, channel);
    }
}
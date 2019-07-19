#ifndef tcp_forward_impl
#define tcp_forward_impl

#include <network/Eventloop.hpp>
#include <network/Epoll.hpp>
#include <network/TcpServer.hpp>
#include <network/TcpClient.hpp>
#include <base/Logger.hpp>
#include <base/Singleton.hpp>
#include <base/Time.hpp>
#include <base/UUID.hpp>

#include <vector>

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

typedef Channel::ChannelPtr ChannelPtr;

void onConnect(const ChannelPtr &, TcpServer *, TcpClient *);
bool onRead(const ChannelPtr &, const char *, int, TcpServer *, TcpClient *);
void onEndReading(const ChannelPtr &, TcpServer *, TcpClient *);
void onEndWriting(const ChannelPtr &, TcpServer *, TcpClient *);
void onClose(const ChannelPtr &);

void onCliConnect(const ChannelPtr &, TcpClient *);
bool onCliRead(const ChannelPtr &, const char *, int, TcpServer *, TcpClient *);
void onCliEndReading(const ChannelPtr &, TcpServer *, TcpClient *);
void onCliEndWriting(const ChannelPtr &, TcpServer *, TcpClient *);
void onCliClose(const ChannelPtr &);


class PipeChannel : public Channel
{
public:
  typedef codechiev::base::UUID UUID;
  typedef boost::weak_ptr<Channel> PipePtr;

  explicit PipeChannel(int);

  UUID::uuid_t session;
  PipePtr pipe;
};

PipeChannel *createPipeChannel(int );

extern const char *port;
extern const char *host;
extern const char *servPort;

#endif
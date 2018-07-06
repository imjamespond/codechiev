#include <libev/TcpServer.hpp>
#include <libev/Channel.hpp> 
#include <base/Logger.hpp>
#include <base/Mutex.hpp>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp> 
 

namespace codechiev{
// namespace base{
//     class Mutex;
// }


namespace libev{

    class ChatRoomServer : public TcpServer
    {
    public:
        ChatRoomServer();

        void broadcast(const char *);
        int totalClient();


        typedef boost::shared_ptr<Channel> channel_ptr;
        typedef boost::weak_ptr<Channel> channel_wptr; 
        typedef boost::unordered_map<int, channel_ptr> ChannelMap;
        ChannelMap channels;

      private: 
        typedef codechiev::base::Mutex::mutex_ptr mutex_ptr;
        mutex_ptr mutex_;

    };
}
}


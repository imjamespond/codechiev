#include <libev/TcpServer.hpp>
#include <libev/Channel.hpp> 
#include <base/Logger.hpp>
#include <base/Mutex.hpp> 
#include <base/BlockedQueue.hpp>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp> 
 
 

typedef codechiev::base::BlockedQueue<1> ChatRoomServerQueue;
namespace codechiev{
namespace libev{

    class ChatRoomServer : public TcpServer
    {
    public:
        ChatRoomServer();

        void broadcast(const char *);
        int totalClient();
        void onMessage(const char*  , int  , Channel * );
        inline void addCount(){++count_;}
        inline int getCount(){return count_;}

        typedef boost::shared_ptr<Channel> channel_ptr;
        typedef boost::weak_ptr<Channel> channel_wptr; 
        // typedef boost::unordered_map<int, channel_ptr> ChannelMap;
        // ChannelMap channels; 
        Channel *channel;
        ChatRoomServerQueue queue;
        codechiev::base::Mutex::mutex_ptr mutex;

      private:

        unsigned int count_;
    };
}
}


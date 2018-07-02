#include <libev/TcpServer.hpp>
#include <libev/Channel.hpp> 
#include <base/Logger.hpp>
#include <base/Mutex.hpp>

#include <boost/bind.hpp>
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

        typedef boost::unordered_map<int, bufferevent_struct *> BuffereventMap;
        BuffereventMap clients;

      private: 
        typedef codechiev::base::Mutex::mutex_ptr mutex_ptr;
        mutex_ptr mutex_;

    };
}
}


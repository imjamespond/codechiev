//
//  EPoll.hpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef EPoll_hpp
#define EPoll_hpp

#include <net/TcpServer.hpp>
#include <boost/noncopyable.hpp>

namespace codechiev {
    namespace net {

        class HttpServer : public TcpServer
        {
        public:
            HttpServer(const std::string& ip, uint16_t port);
            void onMessage(Channel* channel);
        private:
        };
    }
}

#endif /* EPoll_hpp */

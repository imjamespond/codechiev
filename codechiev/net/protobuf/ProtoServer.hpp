//
//  EPoll.hpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef ProtoServer_hpp
#define ProtoServer_hpp

#include <net/TcpEndpoint.hpp>
#include <boost/noncopyable.hpp>

namespace codechiev {
    namespace net {

        class ProtoServer : public TcpServer
        {
        public:
            ProtoServer(const std::string& ip, uint16_t port);
            void onMessage(Channel* channel);
        private:
        };
    }
}

#endif /* HttpServer_hpp */

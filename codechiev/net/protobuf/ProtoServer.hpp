//
//  EPoll.hpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef ProtoServer_hpp
#define ProtoServer_hpp

#include <base/BlockingQueue.hpp>
#include <net/TcpEndpoint.hpp>
#include <net/TcpLengthCoder.h>
#include <boost/noncopyable.hpp>

namespace codechiev {
    namespace net {

        class ProtoServer : public TcpServer
        {
        public:
            ProtoServer(const std::string& ip, uint16_t port);
            void onMessage(const std::string&, int);
            void onData(Channel* channel);
            
            typedef base::BlockingQueue<4> blocking_queue;
            typedef TcpLengthCoder<4> tcplengthcoder;
        private:
            blocking_queue queue_;
        };
    }
}

#endif /* HttpServer_hpp */

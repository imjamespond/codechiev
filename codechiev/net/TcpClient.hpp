//
//  TcpClient.hpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef TcpClient_hpp
#define TcpClient_hpp

#include <boost/noncopyable.hpp>
#include "EPoll.hpp"
#include "EventLoop.h"

namespace codechiev {
    namespace net {
        
        class TcpClient: public boost::noncopyable
        {
        public:
            TcpClient();
            
            void connect(const std::string &, int);
            void pollHandle(const chanenl_vec&);
            
        private:
            EventLoop<EPoll> loop_;
        };
    }
}

#endif /* TcpClient_hpp */

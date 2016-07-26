//
//  TcpServer.hpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef TcpServer_hpp
#define TcpServer_hpp

#include <stdio.h>
#include <boost/noncopyable.hpp>
#include "EPoll.hpp"
#include "EventLoop.h"

namespace codechiev {
    namespace net {
        
        
        
        class TcpServer: public boost::noncopyable
        {
        public:
            TcpServer();
            
            void start();
            void pollHandle(const chanenl_vec&);
            
        private:
            EventLoop<EPoll> loop_;
        };
    }
}


#endif /* TcpServer_hpp */

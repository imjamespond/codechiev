//
//  TcpServer.hpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef TcpServer_hpp
#define TcpServer_hpp

#include <boost/noncopyable.hpp>
#include "EPoll.hpp"
#include "EventLoop.h"

namespace codechiev {
    namespace net {
        
        
        
        class TcpServer: public boost::noncopyable
        {
        public:
            explicit TcpServer(const std::string&, uint16_t );
            
            void start();
            void stop();
            void pollEvent(const chanenl_vec&);
            
        private:
            Channel servchannel_;
            EventLoop<EPoll> loop_;
            std::string ipaddr_;
            uint16_t port_;
        };
    }
}


#endif /* TcpServer_hpp */

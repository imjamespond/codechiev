//
//  ProtoClient.hpp
//  codechie
//
//  Created by metasoft on 16/8/16.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef ProtoClient_hpp
#define ProtoClient_hpp
#include <net/TcpEndpoint.hpp>
#include <net/TcpLengthCoder.h>
namespace codechiev {
    namespace net {
        
        class ProtoClient : public TcpClient
        {
        public:
            typedef boost::function<void(const std::string& msg)> on_message_func;
            typedef TcpLengthCoder<4> tcplengthcoder;
            
            ProtoClient(const std::string& ip, uint16_t port);
            void onMessage(const std::string&, Channel*);
            void onData(Channel* channel);
            
            inline void setOnMessage(const on_message_func& func){onMessage_=func;}
        private:
            on_message_func onMessage_;
        };
    }
}

#endif /* ProtoClient_hpp */

//
//  EPoll.hpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef EPoll_hpp
#define EPoll_hpp

#include "Channel.hpp"
#include <vector>
#include <sys/epoll.h>
#include <boost/noncopyable.hpp>

namespace codechiev {
    namespace net {
        
        class EPoll : public boost::noncopyable
        {
        public:
            EPoll();
            
            void poll(Channel::chanenl_vec&);
            void addEvent(Channel&);
            void setEvent(Channel&);
            
            typedef std::vector<struct epoll_event> epoll_events;
        private:
            Channel epollch_;
            epoll_events events_;
        };
    }
}

#endif /* EPoll_hpp */

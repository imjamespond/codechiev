//
//  Timer.hpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef Timer_hpp
#define Timer_hpp

#include "Channel.hpp"
#include "EventLoop.h"
#include <boost/noncopyable.hpp>
#include <stdint.h>        /* Definition of uint64_t */
#include <stdio.h>

namespace codechiev {
    namespace net {

        class Timer
        {
        public:
            Timer();

            void setTime();
            void after(int64_t);
            void every(int64_t, int64_t);

            inline Channel* getChannel(){return &channel_;}
        private:
            Channel channel_;
        };
        
        class Scheduler : public boost::noncopyable
        {
        public:
            Scheduler();
            
            void pollEvent( channel_vec&);
            void schedule();
            void scheduleTimer(Timer& timer);
        private:
            Channel channel_;
            EventLoop<EPoll> loop_;
        };
        
        
    }
}

#endif /* Timer_hpp */

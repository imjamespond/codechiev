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
#include <boost/noncopyable.hpp>
#include <stdint.h>        /* Definition of uint64_t */

namespace codechiev {
    namespace net {

        class Timer : public boost::noncopyable
        {
        public:
            Timer();

            void setTime();
            void after(int64_t);

            inline Channel& getChannel(){return channel_;}
        private:
            Channel channel_;
        };
    }
}

#endif /* Timer_hpp */

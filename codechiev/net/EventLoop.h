//
//  EventLoop.hpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef EventLoop_hpp
#define EventLoop_hpp

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <base/Thread.hpp>
#include "EPoll.hpp"

namespace codechiev {
    namespace net {

        template<class TPoll>
        class EventLoop : public boost::noncopyable
        {
        public:
            typedef boost::function<void(const channel_vec&)> loop_handle_func;

            explicit EventLoop():
            poll_()
            {
                tid_=base::Thread::Tid();
            }

            void loop(const loop_handle_func& handle)
            {
                assert(tid_!=base::Thread::Tid() && handle);

                while(1)
                {
                    channel_vec vec;
                    poll_.poll(vec);
                    handle(vec);
                }
            }

            inline TPoll& getPoll(){return poll_;}

        private:
            int tid_;
            TPoll poll_;
        };
    }
}

#endif /* EventLoop_hpp */

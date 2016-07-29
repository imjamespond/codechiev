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
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/tuple/tuple.hpp>
#include <set>
#include <stdint.h>        /* Definition of uint64_t */
#include <stdio.h>

namespace codechiev {
    namespace net {

        class Timer : public boost::noncopyable
        {
        public:
            typedef boost::function<void()> timer_cb_t;
            typedef boost::shared_ptr<Timer> timer_ptr_t;
            typedef boost::unordered_map<int, timer_ptr_t> timer_map_t;
            Timer();
            ~Timer();

            void setTime();
            void after(int64_t);
            void after(int64_t, const timer_cb_t&);
            void every(int64_t, int64_t);
            void every(int64_t, int64_t, const timer_cb_t&);

            inline Channel* getChannel(){return &channel_;}
            inline void callback(){if(cb_)cb_();}
        protected:
            Channel channel_;
            timer_cb_t cb_;
        };
        typedef Timer::timer_ptr_t timer_ptr;
        typedef Timer::timer_map_t timer_map;
        
        class TimerQueue : public Timer
        {
        public:
            typedef boost::tuple<int64_t, Timer::timer_cb_t> task_t;
            typedef std::set<task_t > task_set_t;
            TimerQueue();
            
        private:
            task_set_t tasks_;
        };
        
        class Scheduler : public boost::noncopyable
        {
        public:
            Scheduler();
            
            void pollEvent(const channel_vec&);
            void schedule();
            void scheduleTimer(const timer_ptr& timer);
            void unscheduleTimer(int fd);
        private:
            EventLoop<EPoll> loop_;
            timer_map timers_;
        };
        
        
    }
}

#endif /* Timer_hpp */

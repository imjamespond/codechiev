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
#include <base/Mutex.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/tuple/tuple.hpp>
#include <map>
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
            void expireAt(int64_t);

            inline Channel* getChannel(){return &channel_;}
            inline void setCallback(const timer_cb_t& cb){cb_=cb;}
            inline void callback(){if(cb_)cb_();}
            
            int64_t next;
        protected:
            Channel channel_;
            timer_cb_t cb_;
        };
        typedef Timer::timer_ptr_t timer_ptr;
        typedef Timer::timer_map_t timer_map;
        typedef Timer::timer_cb_t timer_cb;
        
        class Scheduler : public boost::noncopyable
        {
        public:
            Scheduler();
            
            void pollEvent(const channel_vec&);
            void schedule();
            void addTimer(const timer_ptr& timer);
            void setTimer(const timer_ptr& timer);
            void removeTimer(int fd);
        private:
            EventLoop<EPoll> loop_;
            timer_map timers_;
        };
        

        class TimerQueue
        {
        public:
            typedef std::vector<timer_cb> task_vec;
            typedef std::multimap<int64_t, timer_cb> task_map;
            typedef std::pair<int64_t, timer_cb> task_pair;
            TimerQueue();
            void commence();
            void addTask(int64_t, const timer_cb&);
            void expire();
        private:
            timer_ptr timer_;
            Scheduler schedule_;
            task_map tasks_;
            base::Mutex mutex_;
        };

    }
}

#endif /* Timer_hpp */

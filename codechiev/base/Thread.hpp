//
//  Thread.hpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef Thread_hpp
#define Thread_hpp

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <pthread.h>

namespace codechiev {
    namespace base {
        class Thread : public boost::noncopyable
        {
        public:
            typedef boost::function<void()> thread_func;
            explicit Thread(const std::string&, const thread_func&);
            ~Thread();
            
            void run();
            void start();
            void join();
            inline std::string getThreadName();
            static std::string ThreadName();
            static int GetTid();
            
        private:
            std::string name_;
            thread_func func_;
            
            pthread_t thread_;
            pthread_attr_t attr_;
        };
        
        inline std::string
        Thread::getThreadName(){return name_;}
    }
}

#endif /* Thread_hpp */

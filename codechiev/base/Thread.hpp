
#ifndef Thread_hpp
#define Thread_hpp

#include <stdint.h>
#include <string>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>

namespace codechiev {
    namespace base {
        class Thread : public boost::noncopyable
        {
        public:
            typedef boost::function<void()> thread_func_t;
            typedef boost::shared_ptr<Thread> thread_ptr_t;
            explicit Thread(const std::string&, const thread_func_t&);
            ~Thread();
            
            void run();
            void start();
            void join();
            void cancel();
            inline std::string getThreadName();
            static std::string ThreadName();
            static int ThreadId();
            
        private:
            std::string name_;
            thread_func_t func_;
            
            pthread_t thread_;
            pthread_attr_t attr_;
        };
        
        inline std::string
        Thread::getThreadName(){return name_;}
        
        typedef Thread::thread_func_t thread_func;
        typedef Thread::thread_ptr_t thread_ptr;
    }
}

#endif /* Thread_hpp */

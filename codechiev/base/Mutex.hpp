#ifndef Mutex_hpp
#define Mutex_hpp

#include <pthread.h>
#include <boost/noncopyable.hpp>

namespace codechiev {
    namespace base {
        
        class Mutex : public boost::noncopyable
        {
        public:
            Mutex();
            Mutex(bool);
            ~Mutex();
            
            void lock();
            void unlock();

            inline pthread_mutex_t *get_pthread_mutex();

          private:
            pthread_mutex_t mutex_;
            pthread_mutexattr_t attr_;
        };
        
        class MutexGuard : public boost::noncopyable
        {
        public:
            explicit MutexGuard(Mutex*);
            ~MutexGuard();
            
        private:
            Mutex* mutex_;
        };

        inline pthread_mutex_t *Mutex::get_pthread_mutex()
        {
            return &mutex_;
        }
    }
}

#endif /* Mutex_hpp */

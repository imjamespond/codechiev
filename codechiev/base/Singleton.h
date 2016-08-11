//
//  Singleton.h
//  codechiev
//
//  Created by metasoft on 16/7/19.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef Singleton_h
#define Singleton_h


#ifdef WIN32

#else
#include <pthread.h>
#include <stdlib.h>
#endif

namespace codechiev
{
    namespace base
    {

#ifdef WIN32
        template <class T>
        class Singleton
        {
        public:
            static T* get()
            {
                InitOnceExecuteOnce(&once_, &Singleton::initRoutine, NULL, NULL);
                return t_;
            }

            static BOOL CALLBACK initRoutine(PINIT_ONCE InitOnce, PVOID Parameter, PVOID *lpContex)
            {
                t_ = new T;
                ::atexit(&Singleton::destructor);
                return TRUE;
            }

            static void destructor()
            {
                delete t_;
                t_ = NULL;
            }

        protected:
        private:
            static T* t_;
            static INIT_ONCE once_;
        };
        template <class T>
        INIT_ONCE Singleton<T>::once_;
        template <class T>
        T* Singleton<T>::t_ = NULL;
#else
        template <class T>
        class Singleton
        {
        public:
            static T* get()
            {
                ::pthread_once(&once_, &Singleton::initRoutine);
                return t_;
            }

            static void initRoutine()
            {
                t_ = new T;
                ::atexit(&Singleton::destructor);
            }

            static void destructor()
            {
                delete t_;
                t_ = NULL;
            }

        protected:
        private:
            static T* t_;
            static pthread_once_t once_;
        };
        template <class T>
        pthread_once_t Singleton<T>::once_ = PTHREAD_ONCE_INIT;
        template <class T>
        T* Singleton<T>::t_ = NULL;
        
        
        
        template <class T>
        class ThreadSingleton
        {
        public:
            static T* get()
            {
                ::pthread_once(&key_once_, &make_key);
                t_ = reinterpret_cast<T*>(::pthread_getspecific(key_)) ;
                if (t_ == NULL)
                {
                    t_ = new T;
                    ::pthread_setspecific(key_, t_);
                }
                
                return t_;
            }
            
            static void make_key()
            {
                ::pthread_key_create(&key_, destructor);
            }
            
            static void destructor(void * )
            {
                if(t_)
                    delete t_;
            }
        private:
            static __thread T* t_;
            static pthread_key_t key_;
            static pthread_once_t key_once_;
        };
        template <class T>
        __thread T* ThreadSingleton<T>::t_ = NULL;
        template <class T>
        pthread_key_t ThreadSingleton<T>::key_;
        template <class T>
        pthread_once_t ThreadSingleton<T>::key_once_;
#endif

    }
}



#endif /* Singleton_h */

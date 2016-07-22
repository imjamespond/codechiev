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
                pthread_once(&once_, &Singleton::initRoutine);
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
#endif
        
    }
}



#endif /* Singleton_h */

#include "Mutex.hpp"
#include <assert.h>

using namespace codechiev::base;

Mutex::Mutex()
{
    ::pthread_mutex_init(&mutex_, NULL);
}

Mutex::Mutex(bool recursive)
{
    if(recursive)
    {
        ::pthread_mutexattr_init(&attr_);
        ::pthread_mutexattr_settype(&attr_, PTHREAD_MUTEX_RECURSIVE);
    }

    ::pthread_mutex_init(&mutex_, &attr_);
}

Mutex::~Mutex()
{
    ::pthread_mutex_destroy(&mutex_);
}

void
Mutex::lock()
{
    ::pthread_mutex_lock(&mutex_);
}

void
Mutex::unlock()
{
    ::pthread_mutex_unlock(&mutex_);
}

MutexGuard::MutexGuard(Mutex *mutex) : mutex_(mutex)
{
    assert(mutex_);
    mutex_->lock();
}

MutexGuard::~MutexGuard()
{
    mutex_->unlock();
}

MutexLock::MutexLock(const Mutex::mutex_ptr &mutex) : mutex_(mutex)
{
    assert(mutex_);
    mutex_->lock();
}

MutexLock::~MutexLock()
{
    mutex_->unlock();
}

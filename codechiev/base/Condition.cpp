#include "Condition.hpp"

using namespace codechiev::base;

Condition::Condition()
{
    //  There is only one attribute defined for condition variables: process-shared, which allows the condition variable to be seen by threads in other processes. The attribute object, if used, must be of type pthread_condattr_t (may be specified as NULL to accept defaults).
    ::pthread_cond_init(&cond_, NULL);
}

Condition::~Condition()
{
    ::pthread_cond_destroy(&cond_);
}

void
Condition::wait(Mutex &mutex)
{
    //  blocks the calling thread until the specified condition is signalled. This routine should be called while mutex is locked, and it will !!!automatically release the mutex!!! while it waits. After signal is received and thread is awakened, mutex will be automatically locked for use by the thread. The programmer is then responsible for unlocking mutex when the thread is finished with it.
    ::pthread_cond_wait(&cond_, mutex.get_pthread_mutex());
}

void
Condition::notify()
{
    //  routine is used to signal (or wake up) another thread which is waiting on the condition variable. !!!!!It should be called after mutex is locked!!!!!, and must unlock mutex in order for pthread_cond_wait() routine to complete.
    ::pthread_cond_signal(&cond_);
}

void
Condition::notifyall()
{
    //  routine should be used instead of pthread_cond_signal() if more than one thread is in a blocking wait state.
    ::pthread_cond_broadcast(&cond_);
}

CountLatch::CountLatch(unsigned short c) : count_(c){}
CountLatch::CountLatch() : count_(1){}
CountLatch::~CountLatch(){}

unsigned short
CountLatch::latch()
{
    MutexGuard lock(&mutex_);

    unsigned short count(count_);//preserve origin count
    while (count_ > 0)
    {
        cond_.wait(mutex_);//release mutex and block here
    }

    return count;
}
unsigned short CountLatch::unlatch()
{
    return this->reduce(1);
}
void
CountLatch::reset(unsigned short val)
{
    MutexGuard lock(&mutex_);

    count_ = val;
}
unsigned short
CountLatch::reduce(unsigned short val)
{
    MutexGuard lock(&mutex_);

    count_ -= val;
    cond_.notifyall();

    return count_;
}
unsigned short
CountLatch::notify(unsigned short val)
{
    MutexGuard lock(&mutex_);

    count_ = val;
    cond_.notify();

    return count_;
}
unsigned short
CountLatch::notifyall(unsigned short val)
{
    MutexGuard lock(&mutex_);

    count_ = val;
    cond_.notifyall();

    return count_;
}

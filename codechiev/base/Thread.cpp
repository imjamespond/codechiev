#include "Thread.hpp"

#include <stdio.h>
#include <errno.h>
#ifdef __linux__
#include <sys/syscall.h>
#endif // __linux__
#ifdef __MINGW32__
#include <windows.h>
#endif // __MINGW32__

using namespace codechiev::base;

#define handle_error_en(en, msg) \
do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)

static void *
start_routine(void *arg)
{
    Thread *thread = static_cast<Thread*>(arg);
    thread->run();
    return NULL;
}

__thread Thread* __this_thread__(NULL);

Thread::Thread(const std::string& name, const thread_func_t& func):
name_(name), func_(func)
{
    //::pthread_attr_setdetachstate(&attr_, PTHREAD_CREATE_JOINABLE);
}

Thread::~Thread()
{
    //::pthread_attr_destroy(&attr_);
}

void
Thread::run()
{
    assert(!__this_thread__);
    __this_thread__ = this;

    if(func_)
    {
        func_();
    }
    //There are several ways in which a thread may be terminated:
    //The thread !!!returns normally!!! from its starting routine. Its work is done.
    //The thread makes a call to the pthread_exit subroutine - !!!whether its work is done or not!!!.
    //The thread is !!!canceled by another thread!!! via the pthread_cancel routine.
    //The entire process is terminated due to making a call to either the exec() or !!!exit()!!!
    //If main() finishes first, without calling pthread_exit explicitly itself
    //The pthread_exit() routine allows the programmer to specify an optional termination status parameter. !!!This optional parameter is typically returned to threads "joining"!!! the terminated thread (covered later).
    ::pthread_exit(NULL);
}

void
Thread::start()
{
    //pthread_create arguments:
    //thread: An opaque, unique identifier for the new thread returned by the subroutine.
    //attr: An opaque attribute object that may be used to set thread attributes. You can specify a thread attributes object, !!!or NULL for the default values!!!.
    //start_routine: the C routine that the thread will execute once it is created.
    //arg: A single argument that may be passed to start_routine. It must be passed by reference as a pointer cast of type void. !!!!NULL may be used if no argument is to be passed!!!!.
    int s;
    s = ::pthread_create( &thread_, 
                          NULL,
                          &start_routine, 
                          this);
    if(s != 0)
    {
        handle_error_en(s, "pthread_create");
    } 
}

void
Thread::join()
{
    //The programmer is able to 
    // obtain the target thread's termination return status
    // if it was !!!!specified in the target thread's call to pthread_exit().
    int s = ::pthread_join(thread_, NULL);
    if(s != 0) 
    {
        handle_error_en(s, "pthread_join");
    }

}

void
Thread::cancel()
{
#ifdef __linux__
    if(::pthread_self() == thread_)
    {
        // printf("thread exit");
        ::pthread_exit(NULL);
    }else
    {
        // printf("thread cancel");
        ::pthread_cancel(thread_);
    }
#endif // __linux__
}

std::string
Thread::ThreadName()
{
    if(__this_thread__)
    {
        return __this_thread__->getThreadName();
    }

    return "__main__";
}

int
Thread::ThreadId()
{
    #ifdef __linux__
    return static_cast<int>(::syscall(SYS_gettid));
    #endif // __linux__

    #ifdef __APPLE__
    uint64_t tid;
    pthread_t self;
    self = ::pthread_self();
    //::pthread_threadid_np(self, &tid);
    return static_cast<int>(tid) ;
    #endif // __APPLE__

    #ifdef __MINGW32__
    return static_cast<int>(::GetCurrentThreadId());
    #endif // __MINGW32__
}

int __main_thread_start__()
{
    return Thread::ThreadId();
}
int __main_thread_id__ = __main_thread_start__();
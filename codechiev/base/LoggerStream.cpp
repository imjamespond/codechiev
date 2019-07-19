#include "Logger.hpp"
#include "Time.hpp"
#include "Thread.hpp"
#include "BlockedQueue.hpp"
#include "Singleton.hpp"

using namespace codechiev::base;

extern int main_thread_id__;

// BlockedQueue<1> __logger_stream_queue__;

void __logger_stream__(LoggerStream::logger_ptr logger)
{
    assert(Thread::GetCurrentThreadId() != main_thread_id__);
    // printf("__logger_stream__\n");// do nothing, leave logger self destruct automaticlly 
}

LoggerStream::LoggerStream(const char *file,
                           const char *func,
                           int line,
                           Logger::Level lv,
                           int carriage,
                           int detail)
{
    logger_ = logger_ptr(new Logger(file, func, line, lv, carriage, detail));
}
 
LoggerStream::~LoggerStream()
{
    // printf("~LoggerStream\n");
    LoggerStreamQueue *q = Singleton<LoggerStreamQueue>::Get();
    q->add(boost::bind(&__logger_stream__, logger_));
}

LoggerStream::LoggerStreamQueue *
LoggerStream::GetQueue()
{
    return Singleton<LoggerStreamQueue>::Get();
}

int logger_stream_queue_start__()
{
    LoggerStream::GetQueue()->start();
    return 0;
}
int logger_stream_queue_init__ = logger_stream_queue_start__();
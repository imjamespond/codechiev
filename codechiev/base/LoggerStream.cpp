#include "Logger.hpp"
#include "Time.hpp"
#include "BlockedQueue.hpp"

using namespace codechiev::base;

BlockedQueue<1> __logger_stream_queue__;
int __logger_stream_queue_start__()
{
    // printf("__logger_stream_queue_start__\n");
    __logger_stream_queue__.start("logger-");
    return 0;
}
int __logger_stream_queue_init__ = __logger_stream_queue_start__();

int __logger_stream__(LoggerStream::logger_ptr logger)
{
    //printf("__logger_stream__\n");// do nothing, leave logger self destruct automaticlly
    return 0;
}

LoggerStream::LoggerStream(const char *file,
                           const char *func,
                           int line,
                           Logger::Level lv,
                           int carriage,
                           int detail)
{
    logger = logger_ptr(new Logger(file, func, line, lv, carriage, detail));
}

LoggerStream::~LoggerStream()
{
    // printf("~LoggerStream\n");
    __logger_stream_queue__.add(boost::bind(&__logger_stream__, logger));
}
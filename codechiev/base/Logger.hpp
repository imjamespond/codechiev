#ifndef Logger_hpp
#define Logger_hpp

#include <stdint.h>
#include <boost/shared_ptr.hpp>

#include <base/FixedBuffer.h> 

namespace codechiev {
    namespace base {

        class Logger
        {
        public:
            enum Level
            {
                Trace,
                Debug,
                Info,
                Warn,
                Error,
                LevelSize,
            };
            explicit Logger(const char* , const char* , int , Level, int, int);
            ~Logger();

            Logger &operator()();
            Logger &operator<<(const void *);
            Logger &operator<<(const char *);
            Logger &operator<<(const unsigned char *);
            Logger &operator<<(const std::string&);
            Logger &operator<<( int);
            Logger &operator<<(uint64_t);
            Logger &operator<<(int64_t);
            Logger &operator<<(uint32_t);
            Logger &operator<<( float);
            Logger &operator<<( double);

        private:
            Level level_;
            typedef FixedBuffer<4096> Buffer;
            Buffer buffer_;
            int carriage_;
            int detail_;
        };
 
        template <int ThreadNum>
        class BlockedQueue;
        class LoggerStream
        {
        public:
            typedef boost::shared_ptr<Logger> logger_ptr;
            logger_ptr logger_;

            explicit LoggerStream(const char* , const char* , int , Logger::Level, int, int);
            ~LoggerStream();

            inline logger_ptr get_logger() { return logger_; }

            typedef BlockedQueue<1> LoggerStreamQueue;
            static LoggerStreamQueue *GetQueue();
        };
    }
}

extern void SetLoggerDetail(unsigned int);
extern void SetLoggerLevel(codechiev::base::Logger::Level);
extern codechiev::base::Logger::Level gLoggerLevel;
extern unsigned int gLoggerDetail;
#define LOG_CHECK(lv) \
    if (lv >= gLoggerLevel) \
    codechiev::base::Logger(__FILE__, __func__, __LINE__, lv, 0, gLoggerDetail)
#define LOG_CHECK_R(lv) \
    if (lv >= gLoggerLevel) \
    codechiev::base::Logger(NULL, NULL, 0, lv, 1, 0)
#define STREAM_CHECK(lv)    \
    if (lv >= gLoggerLevel) \
    codechiev::base::LoggerStream(__FILE__, __func__, __LINE__, lv, 0, gLoggerDetail).get_logger()->operator()()
#define LOG_DEBUG LOG_CHECK(codechiev::base::Logger::Debug)
#define LOG_TRACE LOG_CHECK(codechiev::base::Logger::Trace)
#define LOG_INFO LOG_CHECK(codechiev::base::Logger::Info)
#define LOG_WARN LOG_CHECK(codechiev::base::Logger::Warn)
#define LOG_ERROR LOG_CHECK(codechiev::base::Logger::Error)
#define LOG_INFO_R LOG_CHECK_R(codechiev::base::Logger::Info)

#define STREAM_INFO STREAM_CHECK(codechiev::base::Logger::Info)
#define STREAM_DEBUG STREAM_CHECK(codechiev::base::Logger::Debug)
#define STREAM_TRACE STREAM_CHECK(codechiev::base::Logger::Trace)

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#endif /* Logger_hpp */

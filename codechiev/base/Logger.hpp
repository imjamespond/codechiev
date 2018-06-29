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
            Logger &operator<<(const char*);
            Logger &operator<<(const unsigned char *);
            Logger &operator<<(const std::string&);
            Logger &operator<<( int);
            Logger &operator<<( int64_t);
            Logger &operator<<( float);
            Logger &operator<<( double);

        private:
            Level level_;
            typedef FixedBuffer<256> Buffer;
            Buffer buffer_;
            int carriage_;
            int detail_;
        };

        class LoggerStream
        {
        public:
            typedef boost::shared_ptr<Logger> logger_ptr;
            logger_ptr logger;

            explicit LoggerStream(const char* , const char* , int , Logger::Level, int, int);
            ~LoggerStream();

            inline logger_ptr get_logger(){ return logger;}
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
    codechiev::base::Logger(NULL, NULL, NULL, lv, 1, 0)
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

#endif /* Logger_hpp */

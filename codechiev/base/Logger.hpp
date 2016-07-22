//
//  Logger.hpp
//  codechiev
//
//  Created by metasoft on 16/7/20.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef Logger_hpp
#define Logger_hpp

#include "FixedBuffer.h"

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
            explicit Logger(const char* , const char* , int , Level );
            ~Logger();

            Logger &operator<<(const char*);
            Logger &operator<<(const std::string&);
            Logger &operator<<( int);
            Logger &operator<<( int64_t);
            Logger &operator<<( float);
            Logger &operator<<( double);
        private:
            Level level_;
            FixedBuffer<256> buffer_;
        };

        class LoggerStream
        {
        };
    }
}

extern void setLoggerDetail(unsigned int);
extern void setLoggerLevel(codechiev::base::Logger::Level);
extern codechiev::base::Logger::Level gLevel;
#define LOG_CHECK( lv ) if(lv>=gLevel)\
    Logger(__FILE__,__func__,__LINE__,lv)
#define LOG_DEBUG LOG_CHECK(Logger::Debug)
#define LOG_TRACE LOG_CHECK(Logger::Trace)
#define LOG_INFO LOG_CHECK(Logger::Info)
#define LOG_WARN LOG_CHECK(Logger::Warn)
#define LOG_ERROR LOG_CHECK(Logger::Error)

#endif /* Logger_hpp */
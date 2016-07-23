//
//  Logger.cpp
//  codechiev
//
//  Created by metasoft on 16/7/20.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "Logger.hpp"
#include "Time.hpp"
#include "Thread.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

using namespace codechiev::base;

unsigned int gDetail(0);
enum Detail
{
    DetailThread=1,
    DetailFile=2,
    DetailFunc=4,
};
Logger::Level initLoggerLevel()
{

    #ifdef LoggerThread
        gDetail|=DetailThread;
    #endif // LoggerThread
    #ifdef LoggerFile
        gDetail|=DetailFile;
    #endif // LoggerFile
    #ifdef LoggerFunc
        gDetail|=DetailFunc;
    #endif // LoggerFunc

    if(::getenv("LoggerDebug"))
    {
        return Logger::Debug;
    }else if(::getenv("LoggerTrace"))
    {
        return Logger::Trace;
    }else if(::getenv("LoggerWarn"))
    {
        return Logger::Warn;
    }else if(::getenv("LoggerError"))
    {
        return Logger::Error;
    }

    return Logger::Info;
}
Logger::Level gLevel=initLoggerLevel();

void setLoggerLevel(Logger::Level lv)
{
    gLevel=lv;
}
void setLoggerDetail(unsigned int detail)
{
    gDetail=detail;
}


const char *kLoggerLevels[Logger::LevelSize]=
{
    "trace",
    "debug",
    "info",
    "warn",
    "error"
};

Logger::Logger(const char* file, const char* func, int line, Level lv):
level_(lv)
{
    this->operator<<(kLoggerLevels[lv]);
    if(gDetail&DetailFile)
        this->operator<<(" file:")<<file<<" line:"<<line;
    if(gDetail&DetailFunc)
        this->operator<<(" func:")<<func;
    if(gDetail&DetailThread)
        this->operator<<(" thread:")<<Thread::ThreadName()<<" tid:"<<Thread::Tid();
    this->operator<<(" log:");
}

Logger::~Logger()
{
    this->operator<<(", ")<<Time::GetSimpleString();

    fwrite(buffer_.str(),1,buffer_.readable(),stdout);
    fflush(stdout);
}

Logger&
Logger::operator<<(const char *str)
{
    buffer_.append(str);
    return *this;
}

Logger&
Logger::operator<<(const std::string & str)
{
    buffer_.append(str.c_str());
    return *this;
}

Logger&
Logger::operator<<(int val)
{
    buffer_.append(boost::lexical_cast<std::string>(val).c_str());
    return *this;
}
Logger&
Logger::operator<<(int64_t val)
{
    buffer_.append(boost::lexical_cast<std::string>(val).c_str());
    return *this;
}

Logger&
Logger::operator<<(float val)
{
    buffer_.append(boost::str(boost::format("%1%") % val).c_str());
    return *this;
}

Logger&
Logger::operator<<(double val)
{
    buffer_.append(boost::str(boost::format("%f") % val).c_str());
    return *this;
}

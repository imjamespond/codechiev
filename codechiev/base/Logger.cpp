//
//  Logger.cpp
//  codechiev
//
//  Created by metasoft on 16/7/20.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "Logger.hpp"
#include "Time.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

using namespace codechiev::base;

bool gDetail(false);
Logger::Level initLoggerLevel()
{
    if(::getenv("LoggerDebug"))
    {
        gDetail = true;
        return Logger::Debug;
    }else if(::getenv("LoggerTrace"))
    {
        gDetail = true;
        return Logger::Trace;
    }else if(::getenv("LoggerWarn"))
    {
        return Logger::Warn;
    }else if(::getenv("LoggerError"))
    {
        gDetail = true;
        return Logger::Error;
    }
    
    return Logger::Info;
}
Logger::Level gLevel=initLoggerLevel();

void setLoggerLevel(Logger::Level lv)
{
    gLevel=lv;
}
void setLoggerDetail(bool detail)
{
    gDetail=detail;
}


const char *kLoggerLevels[Logger::LevelSize]=
{
    "Trace",
    "Debug",
    "Info",
    "Warn",
    "Error"
};

Logger::Logger(const char* file, const char* func, int line, Level lv):
level_(lv)
{
    this->operator<<(kLoggerLevels[lv]);
    if(gDetail)
        this->operator<<(" file:")<<file<<" func:"<<func<<" line:"<<line;
    this->operator<<(" :");
}

Logger::~Logger()
{
    this->operator<<(", ")<<Time::GetSimpleString();
    
    printf("%s", buffer_.str());
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
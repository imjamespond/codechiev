#include "Logger.hpp"
#include "Time.hpp"
#include "Thread.hpp"
#include <sstream> 
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp> 

using namespace codechiev::base;

Logger::Level gLoggerLevel;
unsigned int gLoggerDetail(0);
enum Detail
{
    DetailThread=1,
    DetailFile=2,
    DetailFunc=4,
};
int __init_logger_fn__()
{
    #ifdef LoggerThread
        gLoggerDetail|=DetailThread;
    #endif // LoggerThread
    #ifdef LoggerFile
        gLoggerDetail|=DetailFile;
    #endif // LoggerFile
    #ifdef LoggerFunc
        gLoggerDetail|=DetailFunc;
    #endif // LoggerFunc

    //if(::getenv("LoggerDebug"))
    #ifdef LoggerLevel
        gLoggerLevel = Logger::LoggerLevel;
    #else
        gLoggerLevel = Logger::Info;
    #endif // LoggerLevel
    return 0;
}
int __init_logger__ = __init_logger_fn__();

void SetLoggerLevel(Logger::Level lv)
{
    gLoggerLevel=lv;
}
void SetLoggerDetail(unsigned int detail)
{
    gLoggerDetail=detail;
}


const char *kLoggerLevels[Logger::LevelSize]=
{
    "trace",
    "debug",
    "info",
    "warn",
    "error"
};

Logger::Logger(const char *file,
               const char *func,
               int line,
               Level lv,
               int carriage,
               int detail) : 
level_(lv), carriage_(carriage), detail_(detail)
{
    if (carriage_)
    {
        this->operator<<("\r");
    }

    this->operator<<(kLoggerLevels[lv]);

    std::string time = Time::GetSimpleString();
    this->operator<<(": " KMAG);
    buffer_.append(time.c_str(), 11, 8); //time.size()-1

    if(detail&DetailFile)
    {
        std::string str(file);
        std::size_t found = str.find_last_of("/\\");
        this->operator<<("," KGRN " file:") << str.substr(found + 1) << ", line:" << line;
    }

    if(detail&DetailFunc)
        this->operator<<("," KCYN " func:") << func;
    if(detail&DetailThread)
        this->operator<<("," KBLU " thread:") << Thread::GetCurrentThreadName() << ", tid:" << Thread::GetCurrentThreadId();

    this->operator<<(", log:" KWHT);
}

Logger::~Logger()
{
    if (!carriage_)
    {
        // fwrite(buffer_.str(), buffer_.readable_bytes(), 1, stdout);
        // fwrite("\n", 1, 1, stdout);
        // fprintf(stdout, buffer_.str());
        printf("%s\n", buffer_.str());
    }
    else
    {
        printf(buffer_.str());
    }

    fflush(stdout);
}

Logger &
Logger::operator()()
{
    return *this;
}

Logger &
Logger::operator<<(const void *ptr)
{
    std::stringstream ss;
    ss << ptr;
    std::string name = ss.str();
    buffer_.append(name.c_str());
    return *this;
}

Logger &
Logger::operator<<(const char *str)
{
    buffer_.append(str);
    return *this;
}

Logger &
Logger::operator<<(const unsigned char *str)
{
    buffer_.append(reinterpret_cast<const char*>(str));
    return *this;
}

Logger &
Logger::operator<<(const std::string &str)
{
    buffer_.append(str.c_str());
    return *this;
}

Logger &
Logger::operator<<(bool b)
{
    buffer_.append(b ? "true" : "false");
    return *this;
}
Logger&
Logger::operator<<(int val)
{
    buffer_.append(boost::lexical_cast<std::string>(val).c_str());
    return *this;
}
Logger &
Logger::operator<<(int64_t val)
{
    buffer_.append(boost::lexical_cast<std::string>(val).c_str());
    return *this;
}
Logger &
Logger::operator<<(uint64_t val)
{
    buffer_.append(boost::lexical_cast<std::string>(val).c_str());
    return *this;
}
Logger &
Logger::operator<<(uint32_t val)
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

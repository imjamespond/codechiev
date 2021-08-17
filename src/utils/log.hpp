#ifndef Log_hpp
#define Log_hpp

#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>
#include <sstream>
#include "time.hpp"

namespace learn_cpp
{
  class LoggerHelper
  {
  public:
    enum Level
    {
      Trace,
      Debug,
      Info,
      Warn,
      Error,
      Print,
      LevelSize,
    };
  };

  template <class StreamT>
  class Logger
  {
  public:
    explicit Logger(const char *file, const char *func, int line, LoggerHelper::Level lv) : level_(lv)
    {
      ss << Time::LocalTime() << "  FILE:" << file << ", FUNC:" << func << ", LINE:" << line << ", ";
    }
    explicit Logger(LoggerHelper::Level lv) : level_(lv)
    {
    }
    ~Logger()
    {
      ss << std::endl;
      ::write(STDOUT_FILENO, ss.str().c_str(), ss.str().length()); //写到标准输出, printf not work when its in fork child process
    }

    StreamT &operator()()
    {
      return ss;
    }

  private:
    LoggerHelper::Level level_;
    StreamT ss;
  };
}

#define LOG_CHECK(lv) \
  if (lv >= 0)        \
  learn_cpp::Logger<std::stringstream>(__FILE__, __func__, __LINE__, lv)
#define LogTrace LOG_CHECK(learn_cpp::LoggerHelper::Trace)
#define Log learn_cpp::Logger<std::stringstream>(learn_cpp::LoggerHelper::Print)

#endif
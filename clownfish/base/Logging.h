#ifndef _CLOWNFISH_BASE_LOGGING_H_
#define _CLOWNFISH_BASE_LOGGING_H_

#include <clownfish/base/LogStream.h>
#include <clownfish/base/Timestamp.h>
#include <string>

namespace clownfish
{

class Logger
{
public:
    enum LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_OF_LEVELS,
    };

    class BaseFilename
    {
    public:
        template<int N>
        inline BaseFilename(const char (&f)[N])
            : filename(f),
              size(N - 1)
        {
            const char* slash = strrchr(filename, '/');
            if (slash)
            {
                filename = slash + 1;
                size -= static_cast<int>(filename - f);
            }
        }

        explicit BaseFilename(const char* f)
            : filename(f)
        {
            const char* slash = strrchr(filename, '/');
            if (slash)
            {
                filename = slash + 1;
            }
            size = static_cast<int>(strlen(filename));
        }

        const char* filename;
        int size;
    };

    Logger(BaseFilename file, int line, LogLevel level);
    Logger(BaseFilename file, int line, LogLevel level, const char* func);
    Logger(BaseFilename file, int line, bool toAbort);
    ~Logger();

    LogStream& stream() { return impl_.stream_; }

    static LogLevel logLevel();
    static void setLogLevel(LogLevel level);

    typedef void (*OutputFunc)(const char* msg, int len);
    typedef void (*FlushFunc)();
    static void setOutput(OutputFunc);
    static void setFlush(FlushFunc);

private:
    class Impl
    {
    public:
        Impl(Logger::LogLevel level, int log_errno, const BaseFilename &file, int line);
        ~Impl();
        void logTime();

        Timestamp time_;
        LogStream stream_;
        Logger::LogLevel level_;
        int line_;
        BaseFilename file_;
    };

    Impl impl_;
};

#define LOG_TRACE if (clownfish::Logger::logLevel() <= clownfish::Logger::TRACE) \
        clownfish::Logger(__FILE__, __LINE__, clownfish::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (clownfish::Logger::logLevel() <= clownfish::Logger::DEBUG) \
        clownfish::Logger(__FILE__, __LINE__, clownfish::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (clownfish::Logger::logLevel() <= clownfish::Logger::INFO)  \
        clownfish::Logger(__FILE__, __LINE__, clownfish::Logger::INFO).stream()
#define LOG_WARN clownfish::Logger(__FILE__, __LINE__, clownfish::Logger::WARN).stream()
#define LOG_ERROR clownfish::Logger(__FILE__, __LINE__, clownfish::Logger::ERROR).stream()
#define LOG_FATAL clownfish::Logger(__FILE__, __LINE__, clownfish::Logger::FATAL).stream()
#define LOG_SYSERR clownfish::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL clownfish::Logger(__FILE__, __LINE__, true).stream()

} // namespace clownfish

#endif // _CLOWNFISH_BASE_LOGGING_H_

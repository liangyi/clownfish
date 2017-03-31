#include <clownfish/base/Logging.h>
#include <clownfish/base/Timestamp.h>
#include <clownfish/base/Thread.h>
#include <errno.h>
#include <cstring>
#include <assert.h>

using namespace std;

namespace clownfish
{
__thread char t_errnobuf[512];
__thread char t_time[32];
__thread time_t t_lastSec;

const char* strerror_tl(int en)
{
    strerror_r(en, t_errnobuf, sizeof(t_errnobuf));
    return t_errnobuf;
}

const char* LogLevelName[Logger::NUM_OF_LEVELS] =
{
    "TRACE ",
    "DEBUG ",
    "INFO  ",
    "WARN  ",
    "ERROR ",
    "FATAL ",
};

class STR
{
public:
    STR(const char* s, unsigned int l)
        : str(s), len(l)
    {
        assert(strlen(s) == len);
    }

    const char* str;
    const unsigned int len;
};

inline LogStream& operator<<(LogStream& s, STR v)
{
    s.append(v.str, v.len);
    return s;
}

inline LogStream& operator<<(LogStream& s, const Logger::BaseFilename& fn)
{
    s.append(fn.filename, fn.size);
    return s;
}

void defaultLogOutput(const char* msg, int len)
{
    fwrite(msg, 1, len, stdout);
}

void defaultLogFlush()
{
    fflush(stdout);
}

Logger::OutputFunc g_logOutput = defaultLogOutput;
Logger::FlushFunc g_logFlush = defaultLogFlush;
Logger::LogLevel g_logLevel = Logger::DEBUG;

} // namespace clownfish

using namespace clownfish;

Logger::Impl::Impl(Logger::LogLevel level, int log_errno, const BaseFilename& file, int line)
    : time_(Timestamp::now()),
      stream_(),
      level_(level),
      line_(line),
      file_(file)
{
    logTime();
    CurrentThread::tid();
    stream_ << STR(CurrentThread::t_tidString, CurrentThread::t_tidStringLength) << ' ';
    stream_ << STR(LogLevelName[level_], 6);
    stream_ << '[' << file_ << ':' << line_ << STR("] ", 2);
    if (log_errno != 0)
    {
        stream_ << strerror_tl(log_errno) << " [errno=" << log_errno << "] ";
    }
}

Logger::Impl::~Impl()
{
    stream_ << "\n";
    g_logOutput(stream_.buffer().data(), stream_.buffer().size());
    if (level_ == Logger::FATAL)
    {
        g_logFlush();
        abort();
    }
}

void Logger::Impl::logTime()
{
    int64_t usecSinceEpoch = time_.usecSinceEpoch();
    time_t sec = static_cast<time_t>(usecSinceEpoch / Timestamp::kUSecPerSec);
    int usec = static_cast<int>(usecSinceEpoch % Timestamp::kUSecPerSec);

    if (t_lastSec == sec)
    {
        stream_ << t_time;
        stream_ << Fmt(".%06d", usec);
    }
    else
    {
        t_lastSec = sec;
        struct tm t;
        localtime_r(&sec, &t);
        int len = snprintf(t_time, sizeof(t_time), "%4d-%02d-%02d %02d:%02d:%02d",
                           t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                           t.tm_hour, t.tm_min, t.tm_sec);
        assert(len == 19);(void)len;
        stream_ << STR(t_time, len) << Fmt(".%06d", usec);
    }
}

Logger::Logger(BaseFilename file, int line, LogLevel level)
    : impl_(level, 0, file, line)
{
}

Logger::Logger(BaseFilename file, int line, LogLevel level, const char* func)
    :impl_(level, 0, file, line)
{
    impl_.stream_ << '[' << func << "] ";
}

Logger::Logger(BaseFilename file, int line, bool toAbort)
    :impl_(toAbort?FATAL:ERROR, errno, file, line)
{
}

Logger::~Logger()
{
}

Logger::LogLevel Logger::logLevel()
{
    return g_logLevel;
}

void Logger::setLogLevel(LogLevel level)
{
    g_logLevel = level;
}

void Logger::setOutput(OutputFunc func)
{
    g_logOutput = func;
}

void Logger::setFlush(FlushFunc func)
{
    g_logFlush = func;
}

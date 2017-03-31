#ifndef _CLOWNFISH_BASE_LOGFILE_H_
#define _CLOWNFISH_BASE_LOGFILE_H_

#include <clownfish/base/Logging.h>
#include <clownfish/base/FileUtils.h>
#include <sys/time.h>
#include <stdio.h>
#include <atomic>

namespace clownfish
{

class LogFile
{
public:
    enum SHIFT_MODE
    {
        SHIFT_BY_SIZE = 1,
        SHIFT_BY_DATE,
        SHIFT_BY_DATE_HOUR,
    };

    LogFile(const std::string& filepath,
            int maxSize,
            int maxFileNum,
            int flushInterval = 3,
            SHIFT_MODE mode = SHIFT_BY_DATE,
            int checkEveryWriteCount = 1024);

    LogFile(const LogFile&) = delete;
    LogFile& operator=(const LogFile&) = delete;
    ~LogFile();

    void append(const char* line, int len);
    void flush();

private:
    std::string getLogFileName(const time_t now, const int index);
    void open();
    void shiftFile(const time_t now);
    bool isTimeChange(const time_t now);

    std::string filepath_;
    const int maxSize_;
    const int maxFileNum_;
    const int flushInterval_;
    const SHIFT_MODE mode_;

    time_t lastFlushTime_;
    std::atomic<time_t> lastShiftTime_;
    char buf_[128 * 1024];
    int writeCount_;
    const int checkEveryN_;

    FILE* fp_;
};

}

#endif

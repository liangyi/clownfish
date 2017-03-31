#include <clownfish/base/LogFile.h>
#include <fcntl.h>
#include <string>
#include <sstream>
#include <assert.h>
#include <atomic>
#include <sys/types.h>
#include <unistd.h>

using namespace std;
using namespace clownfish;

LogFile::~LogFile()
{
    ::fclose(fp_);
}

LogFile::LogFile(const string& filepath,
                 int maxSize,
                 int maxFileNum,
                 int flushInterval,
                 SHIFT_MODE mode,
                 int checkEveryNWriteCount)
    : filepath_(filepath),
      maxSize_(maxSize),
      maxFileNum_(maxFileNum),
      flushInterval_(flushInterval),
      mode_(mode),
      lastFlushTime_(time(NULL)),
      lastShiftTime_(0),
      writeCount_(0),
      checkEveryN_(checkEveryNWriteCount)
{
    open();
}

void LogFile::flush()
{
    if (fp_)
    {
        ::fflush(fp_);
    }
}

void LogFile::open()
{
    time_t now = time(NULL);
    string filename = getLogFileName(now, 0);
    fp_ = fopen(filename.c_str(), "a");
    assert(fp_);
    setbuffer(fp_, buf_, sizeof(buf_));
}

string LogFile::getLogFileName(time_t now, const int index)
{
    char filename[1024] = {0};
    stringstream suffix;
    struct tm tm_now;
    localtime_r(&now, &tm_now);

    suffix << ".log";
    if (index != 0)
    {
        suffix << "." << index;
    }

    if (mode_ == SHIFT_BY_DATE)
    {
        snprintf(filename, sizeof(filename) - 1, "%s_%04d%02d%02d%s", filepath_.c_str(), tm_now.tm_year + 1900, tm_now.tm_mon + 1, tm_now.tm_mday, suffix.str().c_str());
    }
    else if (mode_ == SHIFT_BY_DATE_HOUR)
    {
        snprintf(filename, sizeof(filename) - 1, "%s_%04d%02d%02d%02d%s", filepath_.c_str(), tm_now.tm_year + 1900, tm_now.tm_mon + 1, tm_now.tm_mday, tm_now.tm_hour, suffix.str().c_str());
    }
    else
    {
        snprintf(filename, sizeof(filename) - 1, "%s%s", filepath_.c_str(), suffix.str().c_str());
    }

    return filename;
}

void LogFile::append(const char* line, int len)
{
    time_t now = time(NULL);

    if (!fp_)
        return;

    fwrite(line, len, 1, fp_);
    writeCount_++;

    if (now - lastFlushTime_ > flushInterval_)
    {
        flush();
        lastFlushTime_ = now;
    }

    bool isTChange = isTimeChange(now);

    if (writeCount_ > checkEveryN_ || isTChange)
    {
        time_t oldTime = atomic_exchange(&lastShiftTime_, now);
        int size = static_cast<int>(ftell(fp_));
        if (oldTime != now && size > maxSize_)
        {
            shiftFile(now);
        }
        writeCount_ = 0;
    }
}

bool LogFile::isTimeChange(const time_t now)
{
    bool isChange = false;
    time_t oldTime = lastShiftTime_.load();
    if (mode_ == SHIFT_BY_DATE)
    {
        isChange = (now - oldTime) / 60 / 60 / 24 != 0;
    }
    else if (mode_ == SHIFT_BY_DATE_HOUR)
    {
        isChange = (now - oldTime) / 60 / 60 != 0;
    }
    return isChange;
}

void LogFile::shiftFile(const time_t now)
{
    string lastIndexFile = getLogFileName(now, maxFileNum_ - 1);
    if (access(lastIndexFile.c_str(), F_OK) == 0)
    {
        // delete the last file
        remove(lastIndexFile.c_str());
    }

    string oldFile, newFile;

    for (int i = maxFileNum_ - 2; i >= 0; --i)
    {
        oldFile = getLogFileName(now, i);
        newFile = getLogFileName(now, i + 1);

        if (access(oldFile.c_str(), F_OK) == 0)
        {
            rename(oldFile.c_str(), newFile.c_str());
        }
    }

    int newFd = ::open(oldFile.c_str(), O_APPEND | O_CREAT | O_WRONLY, 0644);
    if (newFd < 0)
    {
        return;
    }
    int fd = fileno(fp_);

    flush();
    dup2(newFd, fd);

    close(newFd);
}

#include <clownfish/base/LogFile.h>
#include <clownfish/base/Logging.h>
#include <unistd.h>

using namespace clownfish;

LogFile log("logfile_test", 2048 * 1024, 10);

void output(const char* line, int len)
{
    log.append(line, len);
}

void flush()
{
    log.flush();
}

int main()
{
    Logger::setOutput(&output);
    Logger::setFlush(&flush);
    Logger::setLogLevel(Logger::TRACE);

    for (int i = 0; i < 1000000; i++)
    {
        LOG_INFO << "dlkfsldkfjsldfjslfjsldkfj ksldjflksdjflksdjfldsjfljsofjwoejfjsdf";
        usleep(5);
    }
    return 0;
}

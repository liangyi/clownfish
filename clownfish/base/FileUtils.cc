#include <clownfish/base/FileUtils.h>
#include <sys/stat.h>

using namespace clownfish::FileUtils;

int getFileSize(const char* file)
{
    struct stat stStat;

    if (stat(file, &stStat) < 0)
    {
        return -1;
    }

    return static_cast<int>(stStat.st_size);
}

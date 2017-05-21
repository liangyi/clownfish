#include <clownfish/base/Timestamp.h>
#include <cinttypes>

using namespace std;
using namespace clownfish;

string Timestamp::toString() const
{
    char buf[32] = {0};
    int64_t sec = usecSinceEpoch_ / kUSecPerSec;
    int64_t usec = usecSinceEpoch_ % kUSecPerSec;
    snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%" PRId64, sec, usec);
    return buf;
}

string Timestamp::toFmtString(bool showUSec) const
{
    char buf[32] = {0};
    time_t sec = static_cast<time_t>(usecSinceEpoch_ / kUSecPerSec);
    struct tm t;
    localtime_r(&sec, &t);

    if (showUSec)
    {
        int usec = static_cast<int>(usecSinceEpoch_ % kUSecPerSec);
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
                 t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                 t.tm_hour, t.tm_min, t.tm_sec, usec);
    }
    else
    {
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
                 t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                 t.tm_hour, t.tm_min, t.tm_sec);
    }

    return buf;
}

Timestamp Timestamp::now()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    int64_t sec = t.tv_sec;
    return Timestamp(sec * kUSecPerSec + t.tv_usec);
}

Timestamp Timestamp::invalid()
{
    return Timestamp();
}

#ifndef _CLOWNFISH_BASE_TIMESTAMP_H_
#define _CLOWNFISH_BASE_TIMESTAMP_H_

#include <sys/time.h>
#include <time.h>
#include <string>

namespace clownfish
{

class Timestamp
{
public:
    Timestamp() : usecSinceEpoch_(0)
    {}

    Timestamp(int64_t usSinceEpoch)
        : usecSinceEpoch_(usSinceEpoch)
    {}

    std::string toString() const;
    std::string toFmtString(bool showUSec = true) const;

    bool isValid() const { return usecSinceEpoch_ > 0; }

    int64_t usecSinceEpoch() const { return usecSinceEpoch_; }
    time_t secSinceEpoch() const
    {
        return usecSinceEpoch_ / kUSecPerSec;
    }

    static Timestamp now();
    static Timestamp invalid();

    static const int kUSecPerSec = 1000 * 1000;

private:
    int64_t usecSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
    return lhs.usecSinceEpoch() < rhs.usecSinceEpoch();
}

inline bool operator<=(Timestamp lhs, Timestamp rhs)
{
    return lhs.usecSinceEpoch() <= rhs.usecSinceEpoch();
}

inline bool operator>(Timestamp lhs, Timestamp rhs)
{
    return lhs.usecSinceEpoch() > rhs.usecSinceEpoch();
}

inline bool operator>=(Timestamp lhs, Timestamp rhs)
{
    return lhs.usecSinceEpoch() >= rhs.usecSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
    return lhs.usecSinceEpoch() == rhs.usecSinceEpoch();
}

} // namespace coral

#endif // _CLOWNFISH_BASE_TIMESTAMP_H_

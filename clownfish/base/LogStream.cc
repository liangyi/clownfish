#include <clownfish/base/LogStream.h>
#include <algorithm>
#include <type_traits>
#include <cassert>

namespace clownfish
{
namespace detail
{

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
const char hex[] = "1234567890ABCDEF";

template<typename T>
size_t convertToChar(T v, char buf[])
{
    T tmp = v;
    char* p = buf;

    do
    {
        int r = static_cast<int>(tmp % 10);
        *p = zero[r];
        p++;
        tmp /= 10;
    } while (tmp != 0);

    if (v < 0)
    {
        *p = '-';
        p++;
    }
    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

size_t convertHexToChar(uintptr_t v, char buf[])
{
    uintptr_t t = v;
    char* p = buf;

    do
    {
        int r = static_cast<int>(t % 16);
        *p = hex[r];
        p++;
        t /= 16;
    } while (t != 0);
    *p = '\0';

    std::reverse(buf, p);

    return p - buf;
}

} // namespace detail
} // namespace clownfish

using namespace clownfish;
using namespace std;

template<typename T>
void LogStream::formatInteger(T v)
{
    if (buf_.avail() >= kMaxNumSize)
    {
        size_t len = detail::convertToChar(v, buf_.current());
        buf_.add(len);
    }
}

LogStream& LogStream::operator<<(short v)
{
    *this << static_cast<int>(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short v)
{
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator<<(int v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v)
{
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(double v)
{
    if (buf_.avail() >= kMaxNumSize)
    {
        int len = snprintf(buf_.current(), kMaxNumSize, "%.12g", v);
        buf_.add(len);
    }
    return *this;
}

LogStream& LogStream::operator<<(const void* ptr)
{
    uintptr_t v = reinterpret_cast<uintptr_t>(ptr);
    if (buf_.avail() >= kMaxNumSize)
    {
        char* buf = buf_.current();
        buf[0] = '0';
        buf[1] = 'x';
        size_t len = detail::convertHexToChar(v, buf+2);
        buf_.add(len+2);
    }
    return *this;
}

template<typename T>
Fmt::Fmt(const char* fmt, T v)
{
    static_assert(is_arithmetic<T>::value == true, "only arithmetic can be used in this class Fmt");
    size_ = snprintf(data_, sizeof(data_), fmt, v);
    assert(static_cast<size_t>(size_) < sizeof(data_));
}

template Fmt::Fmt(const char* fmt, short);
template Fmt::Fmt(const char* fmt, unsigned short);
template Fmt::Fmt(const char* fmt, int);
template Fmt::Fmt(const char* fmt, unsigned int);
template Fmt::Fmt(const char* fmt, long);
template Fmt::Fmt(const char* fmt, unsigned long);
template Fmt::Fmt(const char* fmt, long long);
template Fmt::Fmt(const char* fmt, unsigned long long);
template Fmt::Fmt(const char* fmt, float);
template Fmt::Fmt(const char* fmt, double);

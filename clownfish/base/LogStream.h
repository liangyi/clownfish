#ifndef _CLOWNFISH_BASE_LOGSTREAM_H_
#define _CLOWNFISH_BASE_LOGSTREAM_H_

#include <string>
#include <cstring>

namespace clownfish
{
namespace detail
{

const int kSmallBuffer = 4096;
const int kLargeBuffer = 4096 * 1024;

template <int SIZE>
class FixedBuffer
{
public:
    FixedBuffer(): cur_(buf_) {}
    ~FixedBuffer() {}
    FixedBuffer& operator=(const FixedBuffer&) = delete;
    FixedBuffer(const FixedBuffer&) = delete;

    int avail() const { return static_cast<int>(end() - cur_); }
    size_t size() const { return static_cast<size_t>(cur_ - buf_); }
    char* current() { return cur_; }
    const char* data() const { return buf_; }
    void bzero() { std::memset(buf_, 0, sizeof(buf_)); }
    void reset() { cur_ = buf_; }
    void append(const char* buf, size_t len)
    {
        int size = static_cast<int>(len);
        if (avail() <= size)
        {
            size = avail() - 1;
        }

        if (size > 0)
        {
            std::memcpy(cur_, buf, size);
            cur_ += size;
        }
    }

    void add(size_t len) { cur_ += len; }

private:
    const char* end() const { return buf_ + sizeof(buf_); }
    char buf_[SIZE];
    char* cur_;
};

} // namespace detail

class LogStream
{
public:
    typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;

    LogStream(): buf_() {}
    LogStream(const LogStream&) = delete;
    LogStream& operator=(const LogStream&) = delete;

    void append(const char* data, int len)
    {
        buf_.append(data, len);
    }
    const Buffer& buffer() const { return buf_; }
    void resetBuffer() { buf_.reset(); }

    LogStream& operator<<(bool v)
    {
        buf_.append(v ? "1" : "0", 1);
        return *this;
    }
    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);
    LogStream& operator<<(float v)
    {
        *this<<static_cast<double>(v);
        return *this;
    }
    LogStream& operator<<(double);
    LogStream& operator<<(const void*);
    LogStream& operator<<(char c)
    {
        buf_.append(&c, 1);
        return *this;
    }
    LogStream& operator<<(const char* str)
    {
        if (str)
        {
            buf_.append(str, std::strlen(str));
        }
        else
        {
            buf_.append("[null]", 6);
        }
        return *this;
    }
    LogStream& operator<<(const unsigned char* str)
    {
        *this << (reinterpret_cast<const char*>(str));
        return *this;
    }
    LogStream& operator<<(const std::string& str)
    {
        buf_.append(str.c_str(), str.size());
        return *this;
    }
private:
    template<typename T>
    void formatInteger(T v);

    Buffer buf_;

    static const int kMaxNumSize = 32;
};

class Fmt
{
public:
    template<typename T>
    Fmt(const char* fmt, T v);

    const char* data() const { return data_; }
    int size() const { return size_; }

private:
    char data_[32];
    int size_;
};

inline LogStream& operator<<(LogStream& s, const Fmt& fmt)
{
    s.append(fmt.data(), fmt.size());
    return s;
}

}
#endif

#include <clownfish/base/Exception.h>
#include <stdlib.h>
#include <execinfo.h>

using namespace std;
using namespace clownfish;

Exception::Exception(const int c, const char* what)
    : errcode_(c),
      message_(what)
{
    fillStackTrace();
}

Exception::Exception(const int c, const string& what)
    : errcode_(c),
      message_(what)
{
    fillStackTrace();
}

Exception::~Exception() throw()
{
}

const char* Exception::what() const throw()
{
    return message_.c_str();
}

const int Exception::errcode() const throw()
{
    return errcode_;
}

const char* Exception::stackTrace() const throw()
{
    return stack_.c_str();
}

void Exception::fillStackTrace()
{
    const int len = 256;
    void* buf[len];
    int n = backtrace(buf, len);
    char** str = backtrace_symbols(buf, n);
    if (str) {
        for (int i = 0; i < n; ++i) {
            stack_.append(str[i]);
            stack_.append("\n");
        }
    }
}

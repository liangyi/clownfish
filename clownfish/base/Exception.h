#ifndef _CLOWNFISH_BASE_EXCEPTION_H_
#define _CLOWNFISH_BASE_EXCEPTION_H_

#include <exception>
#include <string>

namespace clownfish
{
class Exception : public std::exception
{
public:
    explicit Exception(const int errcode, const char* what);
    explicit Exception(const int errcode, const std::string& what);
    virtual ~Exception() throw();
    virtual const char* what() const throw();
    const int errcode() const throw();
    const char* stackTrace() const throw();

private:
    void fillStackTrace();

    int errcode_;
    std::string message_;
    std::string stack_;
};
}

#endif

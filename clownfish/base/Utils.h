#ifndef _CLOWNFISH_BASE_UTILS_H_
#define _CLOWNFISH_BASE_UTILS_H_

#include <string>
#include <sstream>

namespace clownfish
{
namespace Utils
{

template<typename T>
std::string toString(T val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

}
}

#endif

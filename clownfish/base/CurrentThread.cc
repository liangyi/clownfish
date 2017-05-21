#include <clownfish/base/CurrentThread.h>
#include <clownfish/base/Utils.h>

namespace clownfish
{
namespace CurrentThread
{
thread_local std::unique_ptr<std::thread::id> t_tid_ptr;
thread_local std::string t_tid_string;

void cacheTid()
{
    t_tid_ptr.reset(new std::thread::id(std::this_thread::get_id()));
    t_tid_string = clownfish::Utils::toString(*t_tid_ptr);
}

std::string tidString()
{
    if (!t_tid_ptr)
    {
        cacheTid();
    }
    return t_tid_string;
}

}
}

#ifndef _CLOWNFISH_BASE_CURRENTTHREAD_H_
#define _CLOWNFISH_BASE_CURRENTTHREAD_H_

#include <thread>
#include <string>
#include <memory>

namespace clownfish
{
namespace CurrentThread
{

extern thread_local std::unique_ptr<std::thread::id> t_tid_ptr;
extern thread_local std::string t_tid_string;

void cacheTid();
std::string tidString();

}
}
#endif

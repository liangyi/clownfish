#ifndef _CLOWNFISH_BASE_THREAD_H_
#define _CLOWNFISH_BASE_THREAD_H_

#include <pthread.h>
#include <functional>
#include <type_traits>
#include <string>
#include <sys/types.h>
#include <unistd.h>

namespace clownfish
{

namespace CurrentThread
{

extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char* t_threadName;

void cacheTid();

inline int tid()
{
    if (t_cachedTid == 0)
    {
        cacheTid();
    }
    return t_cachedTid;
}

inline const char* tidString()
{
    return t_tidString;
}

inline int tidStringLength()
{
    return t_tidStringLength;
}

inline const char* name()
{
    return t_threadName;
}

} // namespace CurrentThread

class Thread
{
public:
    typedef std::function<void ()> ThreadFunc;

    explicit Thread(ThreadFunc&&, const std::string& name = std::string());
    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;
    ~Thread();

    void start();
    int join();
    bool isStarted() const { return isStarted_; }
    pid_t tid() const { return *tid_; }
    const std::string& name() const { return name_; }

private:
    void setDefaultName();

    bool isStarted_;
    bool isJoined_;
    pthread_t pthreadId_;
    std::shared_ptr<pid_t> tid_;
    std::string name_;
    ThreadFunc func_;

    static std::atomic_int numCreated_;
};
} // namespace clownfish

#endif // _CLOWNFISH_BASE_THREAD_H_

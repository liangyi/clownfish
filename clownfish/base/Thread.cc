#include <clownfish/base/Thread.h>

using namespace std;

namespace clownfish
{
namespace CurrentThread
{
__thread int t_cachedTid = 0;
__thread char t_tidString[32] = {0};
__thread int t_tidStringLength = 6;
__thread const char* t_threadName = "unknown";

static_assert(std::is_same<int, pid_t>::value == true, "int is not same type from pid_t");
} // namespace CurrentThread

namespace detail
{

pid_t gettid()
{
#ifdef OS_PLATFORM_LINUX
    return static_cast<pid_t>(::syscall(SYS_gettid));
#else
    return static_cast<pid_t>(pthread_mach_thread_np(pthread_self()));
#endif
}

void afterFork()
{
    clownfish::CurrentThread::t_cachedTid = 0;
    clownfish::CurrentThread::t_threadName = "main";
    CurrentThread::tid();
}

class ThreadNameInit
{
public:
    ThreadNameInit()
    {
        clownfish::CurrentThread::t_threadName = "main";
        CurrentThread::tid();
        pthread_atfork(NULL, NULL, &afterFork);
    }
};

ThreadNameInit threadNameInit;

class ThreadData
{
public:
    typedef clownfish::Thread::ThreadFunc ThreadFunc;

    ThreadData(const ThreadFunc& func,
              const string& name,
              const shared_ptr<pid_t>& tid)
        : func_(func),
          name_(name),
          wkTid_(tid)
    {}

    void runInThread()
    {
    }
private:
    ThreadFunc func_;
    string name_;
    weak_ptr<pid_t> wkTid_;
};

void* threadEntry(void* arg)
{
    shared_ptr<ThreadData> thread(static_cast<ThreadData*>(arg));
    thread->runInThread();
    return NULL;
}

} // namespace detail
} // namespace clownfish

using namespace clownfish;

atomic_int Thread::numCreated_;

void CurrentThread::cacheTid()
{
    if (t_cachedTid == 0) {
        t_cachedTid = detail::gettid();
        t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString), "%5d", t_cachedTid);
    }
}

Thread::Thread(ThreadFunc&& func, const string& name)
    : isStarted_(false),
      isJoined_(false),
      pthreadId_(0),
      tid_(new pid_t(0)),
      name_(name)
{
    setDefaultName();
}

void Thread::setDefaultName()
{
    int num = atomic_fetch_add(&numCreated_, 1);
    if (name_.empty()) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name_ = buf;
    }
}

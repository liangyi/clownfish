#include <clownfish/base/Thread.h>
#include <clownfish/base/Logging.h>
#include <clownfish/base/Exception.h>
#include <assert.h>
#include <stdio.h>

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
        pid_t tid = clownfish::CurrentThread::tid();

        shared_ptr<pid_t> ptid = wkTid_.lock();
        if (ptid)
        {
            *ptid = tid;
            ptid.reset();
        }

        clownfish::CurrentThread::t_threadName = name_.empty() ? "clownfishThread" : name_.c_str();

        try
        {
            func_();
            clownfish::CurrentThread::t_threadName = "finished";
        }
        catch (const Exception& ex)
        {
            clownfish::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "Exception caught in Thread[%s]\n", name_.c_str());
            fprintf(stderr, "reason[errcode: %d]: %s\n", ex.errcode(), ex.what());
            fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
            abort();
        }
        catch (const std::exception& ex)
        {
            clownfish::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "Exception caught in Thread[%s]\n", name_.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            abort();
        }
        catch (...)
        {
            clownfish::CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "Exception caught in Thread[%s]\n", name_.c_str());
            throw;
        }
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
      name_(name),
      func_(std::move(func))
{
    setDefaultName();
}

Thread::~Thread()
{
    if (isStarted_ && !isJoined_)
    {
        pthread_detach(pthreadId_);
    }
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

void Thread::start()
{
    assert(!isStarted_);
    isStarted_ = true;
    detail::ThreadData* data = new detail::ThreadData(func_, name_, tid_);

    if (pthread_create(&pthreadId_, NULL, &detail::threadEntry, data))
    {
        isStarted_ = false;
        delete data;
        LOG_SYSFATAL << "pthread_create failed";
    }
}

int Thread::join()
{
    assert(isStarted_);
    assert(!isJoined_);
    isJoined_ = true;
    return pthread_join(pthreadId_, NULL);
}

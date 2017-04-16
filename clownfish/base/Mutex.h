#ifndef _CLOWNFISH_BASE_MUTEX_H_
#define _CLOWNFISH_BASE_MUTEX_H_

#include <pthread.h>
#include <assert.h>

#define RET_CHECK(ret) ({ auto errcode = (ret); \
                assert(errcode == 0); (void) errcode;})

namespace clownfish
{

class Mutex
{
public:
    Mutex()
    {
        RET_CHECK(pthread_mutex_init(&mutex_, NULL));
    }
    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;
    ~Mutex()
    {
        RET_CHECK(pthread_mutex_destroy(&mutex_));
    }

    void lock()
    {
        RET_CHECK(pthread_mutex_lock(&mutex_));
    }
    void unlock()
    {
        RET_CHECK(pthread_mutex_unlock(&mutex_));
    }

private:
    pthread_mutex_t mutex_;
};

class MutexLockGuard
{
public:
    explicit MutexLockGuard(Mutex& m)
        : mutex_(m)
    {
        mutex_.lock();
    }
    MutexLockGuard(const MutexLockGuard&) = delete;
    MutexLockGuard& operator=(const MutexLockGuard&) = delete;

    ~MutexLockGuard()
    {
        mutex_.unlock();
    }

private:
    Mutex& mutex_;
};

class Rwlock
{
public:
    Rwlock(bool isWriterPrefer = false)
    {
        if (isWriterPrefer)
        {
            RET_CHECK(pthread_rwlockattr_init(&attr_));
#ifdef OS_PLATFORM_LINUX
            RET_CHECK(pthread_rwlockattr_setkind_np(&attr_, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP));
#endif
            RET_CHECK(pthread_rwlock_init(&rwlock_, &attr_));
        }
        else
        {
            RET_CHECK(pthread_rwlock_init(&rwlock_, NULL));
        }
    }
    Rwlock(const Rwlock&) = delete;
    Rwlock& operator=(const Rwlock&) = delete;
    ~Rwlock()
    {
        RET_CHECK(pthread_rwlock_destroy(&rwlock_));
    }

    void rdlock()
    {
        RET_CHECK(pthread_rwlock_rdlock(&rwlock_));
    }
    void wrlock()
    {
        RET_CHECK(pthread_rwlock_wrlock(&rwlock_));
    }
    void unlock()
    {
        RET_CHECK(pthread_rwlock_unlock(&rwlock_));
    }

private:
    pthread_rwlock_t rwlock_;
    pthread_rwlockattr_t attr_;
};

class RdlockGuard
{
public:
    explicit RdlockGuard(Rwlock& rw)
        : rwlock_(rw)
    {
        rwlock_.rdlock();
    }
    RdlockGuard(const RdlockGuard&) = delete;
    RdlockGuard& operator=(const RdlockGuard&) = delete;
    ~RdlockGuard()
    {
        rwlock_.unlock();
    }

private:
    Rwlock& rwlock_;
};

class WrlockGuard
{
public:
    explicit WrlockGuard(Rwlock& rw)
        : rwlock_(rw)
    {
        rwlock_.wrlock();
    }
    WrlockGuard(const WrlockGuard&) = delete;
    WrlockGuard& operator=(const WrlockGuard&) = delete;
    ~WrlockGuard()
    {
        rwlock_.unlock();
    }

private:
    Rwlock& rwlock_;
};

}

#endif // _CLOWNFISH_BASE_MUTEX_H_

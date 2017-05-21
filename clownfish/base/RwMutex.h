#ifndef _CLOWNFISH_BASE_RWMUTEX_H_
#define _CLOWNFISH_BASE_RWMUTEX_H_

#include <pthread.h>
#include <assert.h>

#define RET_CHECK(ret) ({ auto errcode = (ret); \
                assert(errcode == 0); (void) errcode;})

namespace clownfish
{

class RwMutex
{
public:
    RwMutex(bool isWriterPrefer = false)
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
    RwMutex(const Rwlock&) = delete;
    RwMutex& operator=(const Rwlock&) = delete;
    ~RwMutex()
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
    explicit RdlockGuard(RwMutex& rw)
        : rwMutex_(rw)
    {
        rwMutex_.rdlock();
    }
    RdlockGuard(const RdlockGuard&) = delete;
    RdlockGuard& operator=(const RdlockGuard&) = delete;
    ~RdlockGuard()
    {
        rwMutex_.unlock();
    }

private:
    RwMutex& rwMutex_;
};

class WrlockGuard
{
public:
    explicit WrlockGuard(RwMutex& rw)
        : rwMutex_(rw)
    {
        rwMutex_.wrlock();
    }
    WrlockGuard(const WrlockGuard&) = delete;
    WrlockGuard& operator=(const WrlockGuard&) = delete;
    ~WrlockGuard()
    {
        rwMutex_.unlock();
    }

private:
    RwMutex& rwMutex_;
};

}

#endif // _CLOWNFISH_BASE_MUTEX_H_

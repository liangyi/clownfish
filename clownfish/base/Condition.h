#ifndef _CLOWNFISH_BASE_CONDITION_H_
#define _CLOWNFISH_BASE_CONDITION_H_

#include <clownfish/base/Mutex.h>
#include <pthread.h>

namespace clownfish
{

class Condition
{
public:
    explicit Condition(Mutex& mutex)
        : mutex_(mutex)
    {
        RET_CHECK(pthread_cond_init(&cond_, NULL));
    }
    ~Condition()
    {
        RET_CHECK(pthread_cond_destroy(&cond_));
    }
    Condition(const Condition&) = delete;
    Condition& operator=(const Condition&) = delete;

    void wait()
    {
        RET_CHECK(pthread_cond_wait(&cond_, mutex_.getMutexPtr()));
    }

    void notify()
    {
        RET_CHECK(pthread_cond_signal(&cond_));
    }

    void notifyAll()
    {
        RET_CHECK(pthread_cond_broadcast(&cond_));
    }

private:
    Mutex& mutex_;
    pthread_cond_t cond_;
};
}

#endif

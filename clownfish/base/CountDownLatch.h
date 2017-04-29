#ifndef _CLOWNFISH_BASE_COUNTDOWNLATCH_H_
#define _CLOWNFISH_BASE_COUNTDOWNLATCH_H_

#include <clownfish/base/Mutex.h>
#include <clownfish/base/Condition.h>

namespace clownfish
{

class CountDownLatch
{
public:
    explicit CountDownLatch(int count);
    CountDownLatch(const CountDownLatch&) = delete;
    CountDownLatch& operator=(const CountDownLatch&) = delete;
    void wait();
    void countDown();
    int getCount() const;

private:
    mutable Mutex mutex_;
    Condition cond_;
    int count_;
};
}

#endif

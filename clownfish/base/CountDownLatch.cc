#include <clownfish/base/CountDownLatch.h>

using namespace clownfish;

CountDownLatch::CountDownLatch(int count)
    : mutex_(),
      cond_(mutex_),
      count_(count)
{
}

void CountDownLatch::wait()
{
    MutexLockGuard guard(mutex_);
    while (count_ > 0)
    {
        cond_.wait();
    }
}

void CountDownLatch::countDown()
{
    MutexLockGuard guard(mutex_);
    --count_;
    if (count_ == 0)
    {
        cond_.notifyAll();
    }
}

int CountDownLatch::getCount() const
{
    MutexLockGuard guard(mutex_);
    return count_;
}

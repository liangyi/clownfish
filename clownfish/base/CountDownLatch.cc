#include <clownfish/base/CountDownLatch.h>

using namespace clownfish;

CountDownLatch::CountDownLatch(int count)
    : mutex_(),
      cond_(),
      count_(count)
{
}

void CountDownLatch::wait()
{
    std::unique_lock<std::mutex> lk(mutex_);
    while (count_ > 0)
    {
        cond_.wait(lk);
    }
}

void CountDownLatch::countDown()
{
    std::unique_lock<std::mutex> lk(mutex_);
    --count_;
    if (count_ == 0)
    {
        cond_.notify_one();
    }
}

int CountDownLatch::getCount() const
{
    std::unique_lock<std::mutex> lk(mutex_);
    return count_;
}

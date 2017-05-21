#ifndef _CLOWNFISH_BASE_COUNTDOWNLATCH_H_
#define _CLOWNFISH_BASE_COUNTDOWNLATCH_H_

#include <mutex>
#include <condition_variable>

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
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    int count_;
};
}

#endif

#ifndef _CLOWNFISH_BASE_THREADPOOL_H_
#define _CLOWNFISH_BASE_THREADPOOL_H_

#include <clownfish/base/Condition.h>
#include <clownfish/base/Thread.h>
#include <memory>
#include <functional>
#include <vector>
#include <deque>

namespace clownfish
{
class ThreadPool
{
public:
    typedef std::function<void ()> Task;

    explicit ThreadPool(const std::string& name = std::string("ThreadPool"));
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
    void setThreadInitCallback(const Task& cb)
    {
        threadInitCallback_ = cb;
    }

    void start(int threadNum);
    void stop();

    const std::string& name() const { return name_; }

    size_t queueSize() const;

    void run(Task&& t);

private:
    bool isFull() const;
    void runInThread();
    Task take();

    mutable Mutex mutex_;
    Condition notEmpty_;
    Condition notFull_;
    std::string name_;
    Task threadInitCallback_;
    std::vector<std::unique_ptr<Thread> > threads_;
    std::deque<Task> queue_;
    size_t maxQueueSize_;
    bool isRunning_;
};
}
#endif

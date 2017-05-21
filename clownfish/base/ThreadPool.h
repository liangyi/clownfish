#ifndef _CLOWNFISH_BASE_THREADPOOL_H_
#define _CLOWNFISH_BASE_THREADPOOL_H_

#include <condition_variable>
#include <thread>
#include <memory>
#include <functional>
#include <vector>
#include <deque>
#include <string>

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

    mutable std::mutex mutex_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    std::string name_;
    Task threadInitCallback_;
    std::vector<std::unique_ptr<std::thread> > threads_;
    std::deque<Task> queue_;
    size_t maxQueueSize_;
    bool isRunning_;
};
}
#endif

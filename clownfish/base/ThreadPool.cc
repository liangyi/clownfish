#include <clownfish/base/ThreadPool.h>
#include <clownfish/base/Exception.h>
#include <assert.h>

using namespace clownfish;
using namespace std;

ThreadPool::ThreadPool(const string& name)
    : mutex_(),
      notEmpty_(mutex_),
      notFull_(mutex_),
      name_(name),
      maxQueueSize_(0),
      isRunning_(false)
{
}

ThreadPool::~ThreadPool()
{
    if (isRunning_)
    {
        stop();
    }
}

void ThreadPool::start(int threadNum)
{
    assert(threads_.empty());
    isRunning_ = true;
    threads_.reserve(threadNum);

    for (int i = 0; i < threadNum; ++i) {
        char id[32];
        snprintf(id, sizeof(id), "%d", i+1);
        unique_ptr<Thread> t(new Thread(bind(&ThreadPool::runInThread, this), name_ + id));
        threads_.push_back(std::move(t));
        threads_[i]->start();
    }
    if (threadNum == 0 && threadInitCallback_)
    {
        threadInitCallback_();
    }
}

void ThreadPool::stop()
{
    {
        MutexLockGuard guard(mutex_);
        isRunning_ = false;
        notEmpty_.notifyAll();
    }
    for(auto& thread: threads_)
    {
        thread->join();
    }
}

size_t ThreadPool::queueSize() const
{
    MutexLockGuard guard(mutex_);
    return queue_.size();
}

void ThreadPool::run(Task&& task)
{
    if (threads_.empty())
    {
        task();
    }
    else
    {
        MutexLockGuard guard(mutex_);
        while (isFull())
        {
            notFull_.wait();
        }
        assert(!isFull());

        queue_.push_back(std::move(task));
        notEmpty_.notify();
    }
}

ThreadPool::Task ThreadPool::take()
{
    MutexLockGuard guard(mutex_);

    while (queue_.empty() && isRunning_)
    {
        notEmpty_.wait();
    }

    Task task;
    if (!queue_.empty())
    {
        task = queue_.front();
        queue_.pop_front();
        if (maxQueueSize_ > 0)
        {
            notFull_.notify();
        }
    }

    return task;
}

bool ThreadPool::isFull() const
{
    return maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_;
}

void ThreadPool::runInThread()
{
    try
    {
        if (threadInitCallback_)
        {
            threadInitCallback_();
        }

        while (isRunning_)
        {
            Task task(take());
            if (task)
            {
                task();
            }
        }
    }
    catch (const Exception& ex)
    {
        fprintf(stderr, "Exception caught in ThreadPool[%s]\n", name_.c_str());
        fprintf(stderr, "reason[errcode: %d]: %s\n", ex.errcode(), ex.what());
        fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
        abort();
    }
    catch (const std::exception& ex)
    {
        fprintf(stderr, "Exception caught in ThreadPool[%s]\n", name_.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        abort();
    }
    catch (...)
    {
        fprintf(stderr, "Exception caught in ThreadPool[%s]\n", name_.c_str());
        throw;
    }
}

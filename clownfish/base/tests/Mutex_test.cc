#include <clownfish/base/Mutex.h>
#include <clownfish/base/Thread.h>
#include <stdio.h>

using namespace clownfish;

void threadfunc()
{
    printf("pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
}

int main()
{
    Mutex mutex;
    Rwlock rwlock;

    MutexLockGuard mlg(mutex);
    WrlockGuard wrg(rwlock);

    Thread t1(threadfunc, "test");
    Thread t2(threadfunc, "test2");
    t1.start();
    t2.start();
    t1.join();
    t2.join();

    return 0;
}

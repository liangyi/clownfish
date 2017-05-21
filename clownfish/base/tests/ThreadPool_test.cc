#include <clownfish/base/ThreadPool.h>
#include <clownfish/base/CountDownLatch.h>
#include <clownfish/base/Logging.h>
#include <clownfish/base/LockFreeQueue.h>
#include <unistd.h>

using namespace std;
using namespace clownfish;

void print(int i)
{
    LOG_INFO << "test pool task " << i;
    //usleep(10);
}

int main()
{
    ThreadPool pool("testPool");

    LockFreeQueue<int> queue;

    //pool.setMaxQueueSize(10);
    pool.start(8);

    for (int i = 1; i <= 50; i++)
    {
        ThreadPool::Task t = bind(&print, i);
        pool.run(std::move(t));
    }

    CountDownLatch latch(1);
    pool.run(bind(&CountDownLatch::countDown, &latch));
    latch.wait();

    pool.stop();

    return 0;
}

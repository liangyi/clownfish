#include <clownfish/base/LockFreeQueue.h>
#include <iostream>

using namespace std;
using namespace clownfish;

int main()
{
    LockFreeQueue<int> queue;

    for (int i = 0; i <= 20; ++i)
    {
        int x = i;
        queue.pushBack(move(x));
    }

    int d = 0;
    while (queue.popFront(d))
    {
        cout <<d << endl;
    }

    return 0;
}

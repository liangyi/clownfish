#include <clownfish/base/Timestamp.h>
#include <iostream>

using namespace std;
using namespace clownfish;

int main()
{
    Timestamp t = Timestamp::now();

    cout << t.toFmtString() << endl;
}

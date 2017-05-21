#include <clownfish/base/Logging.h>
#include <clownfish/base/CurrentThread.h>

using namespace clownfish;

void logtest()
{
    LOG_TRACE << "trace code";
    LOG_DEBUG << "debug the code";
    LOG_INFO << "the tid is " << CurrentThread::tidString();
    LOG_ERROR << "error ?";
}

int main()
{
    Logger::setLogLevel(Logger::TRACE);

    std::thread t1(logtest);
    std::thread t2(logtest);

    LOG_INFO << "the main tid is " << CurrentThread::tidString();

    t1.join();
    t2.join();

    return 0;
}

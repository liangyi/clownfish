#include <clownfish/base/Logging.h>
#include <clownfish/base/Thread.h>

using namespace clownfish;

int main()
{
    Logger::setLogLevel(Logger::TRACE);
    LOG_TRACE << "trace code";
    LOG_DEBUG << "debug the code";
    LOG_INFO << "the tid is " << CurrentThread::tid();
    LOG_ERROR << "error ?";

    return 0;
}

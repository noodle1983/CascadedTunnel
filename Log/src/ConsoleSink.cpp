#include "ConsoleSink.h"
#include "Log.h"
#include <fstream>
#include <iostream>

using namespace nd;
using namespace std;

static inline std::tm localtime_nd(std::time_t timer)
{
    std::tm bt;
#if defined(_MSC_VER)
    localtime_s(&bt, &timer);
#else
    localtime_r(&timer, &bt);
#endif
    return bt;
}
//-----------------------------------------------------------------------------

ConsoleSink::ConsoleSink(Severity severity)
    : timeFormatM("%Y-%m-%d_%H-%M-%S")
    , severityM(severity)
{
}

//-----------------------------------------------------------------------------

void ConsoleSink::log(const LogMeta* theMeta)
{
    if (theMeta->severityM < severityM) {return ;}

    std::time_t now = std::chrono::system_clock::to_time_t(theMeta->timepointM);
    struct ::tm nowTm = localtime_nd(now);
    char fileTimeStr[128];
    strftime(fileTimeStr, sizeof(fileTimeStr), timeFormatM.c_str(), &nowTm);

    int ms = chrono::time_point_cast<chrono::milliseconds>(theMeta->timepointM).time_since_epoch().count() % 1000;
    char logTimeStr[256];
    snprintf(logTimeStr, sizeof(logTimeStr), "%s.%03d", fileTimeStr, ms);

    const char* logTypeStr = (theMeta->logTypeM == LOG_TYPE_CFG) ? " CFG " : "";
    cout << logTimeStr << logTypeStr << severityToStr((Severity)theMeta->severityM) 
        << "(" << theMeta->lineInfoM.filenameM << ":" << theMeta->lineInfoM.linenoM << ")" 
        << theMeta->streamM.str() << endl << flush;

}

//-----------------------------------------------------------------------------


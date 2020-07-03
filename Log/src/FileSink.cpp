#include "FileSink.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace nd;

inline std::tm localtime_nd(std::time_t timer)
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

FileSink::FileSink(std::string& prefix, int keepno, Severity severity)
    : prefixM(prefix)
    , keepnoM(keepno)
    , timeFormatM("%Y-%m-%d_%H-%M-%S")
    , severityM(severity)
{
}

//-----------------------------------------------------------------------------

void FileSink::log(const LogMeta* theMeta)
{
    if (theMeta->severityM < severityM) {return ;}

    std::time_t now = std::chrono::system_clock::to_time_t(theMeta->timepointM);
    struct ::tm nowTm = localtime_nd(now);
    char fileTimeStr[256];
    strftime(fileTimeStr, sizeof(fileTimeStr), timeFormatM.c_str(), &nowTm);

    if (!fileHandleM.is_open())
    {
        string filename = prefixM + "_" + fileTimeStr + ".log";
        fileHandleM.open(filename.c_str(), std::ios_base::out|std::ios_base::app);
        if (!fileHandleM.good()){
            cerr << "failed to open file[" << filename << "] errno:" << errno << endl;
            abort();
        }
    }
    int ms = chrono::time_point_cast<chrono::milliseconds>(theMeta->timepointM).time_since_epoch().count() % 1000;
    char logTimeStr[256];
    snprintf(logTimeStr, sizeof(logTimeStr), "%s.%03d", fileTimeStr, ms);

    // log format: time severity (file:no) msg
    fileHandleM << logTimeStr << severityToStr((Severity)theMeta->severityM) 
        << "(" << theMeta->lineInfoM.filenameM << ":" << theMeta->lineInfoM.linenoM << ")" 
        << theMeta->streamM.str() << endl << flush;

}


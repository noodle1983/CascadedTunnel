#include "LogStruct.h"
namespace nd { thread_local nd::ThreadLocalLogMeta tl_logmeta; }
#include "Logger.h"
#include "ConfigCenter.h"
#include "BoostProcessor.h"
#include "FileSink.h"

using namespace nd;
using namespace Config;
using namespace std;


//-----------------------------------------------------------------------------

Logger::Logger()
    : minSeverityM((int)Severity::Trace)
{
}

//-----------------------------------------------------------------------------

Logger::~Logger()
{
    g_io_processor->waitStop();
    for (Sink* sink : sinksM)
    {
        delete sink;
    }
    sinksM.clear();
}

//-----------------------------------------------------------------------------

void Logger::init()
{
    int logLevel = g_cfg->get("log.level", (int)Severity::Debug);
    //bool runInBackground = g_cfg->get("process.background", 1);
    std::string logFilename = g_cfg->get("log.filename", "trouble_shooting_");
    int fileNum = g_cfg->get("log.fileNum", 10);

    //minSeverityM = logLevel;
    sinksM.push_back(new FileSink(logFilename, fileNum, (Severity)logLevel));
}

//-----------------------------------------------------------------------------

bool Logger::willAccept()
{
    if (tl_logmeta.severityM < minSeverityM) {return false;}

    return true;
}

//-----------------------------------------------------------------------------

void Logger::handleLogMeta(LogMeta* meta)
{
    for (Sink* sink : sinksM)
    {
        sink->log(meta);
    }
    delete meta;
}

//-----------------------------------------------------------------------------

template<>
Logger& nd::operator<<(Logger& theLogger, const Severity& severity) {
    tl_logmeta.severityM = severity;
    return theLogger;
}

template<> Logger& nd::operator<<(Logger& theLogger, const Condition& condition){
    tl_logmeta.ignoreM = !condition.isLogM;
    return theLogger;
}

template<> Logger& nd::operator<<(Logger& theLogger, const SrcLine& srcline){
    tl_logmeta.lineInfoM.filenameM = std::move(srcline.filenameM);
    tl_logmeta.lineInfoM.linenoM = std::move(srcline.linenoM);
    return theLogger;
}

template<>
Logger& nd::operator<<(Logger& theLogger, const LogEnd& end) {
    if(tl_logmeta.ignoreM || !theLogger.willAccept()){
        tl_logmeta.clear();
        return theLogger;
    }

    LogMeta* meta = new LogMeta(std::move(tl_logmeta));
    g_io_processor->PROCESS(0, &Logger::handleLogMeta, &theLogger, meta);
    return theLogger;
}


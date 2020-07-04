#include "LogStruct.h"
namespace nd { thread_local nd::ThreadLocalLogMeta tl_logmeta; }
#include "Logger.h"
#include "ConfigCenter.h"
#include "Processor.h"
#include "FileSink.h"
#include "ConsoleSink.h"

using namespace nd;
using namespace std;


//-----------------------------------------------------------------------------

Logger::Logger(int logType)
    : minSeverityM((int)Severity::Trace)
{
    if (logType == LOG_TYPE_CFG){
        initCfgLog();
    }else{
        initNormalLog();
    }
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

void Logger::initCfgLog()
{
    minSeverityM = Severity::Trace;
    string prefix("cfg");
    sinksM.push_back(new FileSink(prefix, 0, (Severity)minSeverityM));
    sinksM.push_back(new ConsoleSink((Severity)minSeverityM));
}

//-----------------------------------------------------------------------------
void Logger::initNormalLog()
{
    int logLevel = g_cfg->get("log.level", (int)Severity::Debug);
    bool runInBackground = g_cfg->get("process.background", 1);
    std::string logFilename = g_cfg->get("log.filename", "trouble_shooting");
    int fileNum = g_cfg->get("log.fileNum", 10);

    minSeverityM = logLevel;
    sinksM.push_back(new FileSink(logFilename, fileNum, (Severity)logLevel));
    if (!runInBackground){
        sinksM.push_back(new ConsoleSink((Severity)logLevel));
    }
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


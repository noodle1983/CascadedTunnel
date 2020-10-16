#include "LogStruct.h"
namespace nd { thread_local nd::ThreadLocalLogMeta tl_logmeta; }
#include "Logger.h"
#include "ConfigCenter.h"
#include "Processor.h"
#include "FileSink.h"
#include "ConsoleSink.h"
#include "App.h"

using namespace nd;
using namespace std;


//-----------------------------------------------------------------------------

Logger::Logger(int logType)
    : minSeverityM((int)Severity::Trace)
    , logTypeM(logType)
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
    fini();
}

//-----------------------------------------------------------------------------

void Logger::fini()
{
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
    FileSink* fileSink = new FileSink(prefix, Severity::Trace);
    fileSink->setKeepNo(0);
    fileSink->setSwitchDays(0);

    sinksM.push_back(fileSink);

    bool runInBackground = g_app->runInBackground();
    if (!runInBackground){
        sinksM.push_back(new ConsoleSink((Severity)minSeverityM));
    }
}

//-----------------------------------------------------------------------------
void Logger::initNormalLog()
{
    int logLevel = g_cfg->get("log.level", (int)Severity::Debug);
    std::string logFilename = g_cfg->get("log.filename", "trouble_shooting");
    int fileNum = g_cfg->get("log.fileNum", 10);
    int switchDays = g_cfg->get("log.switchday", 1);

    minSeverityM = logLevel;
    FileSink* fileSink = new FileSink(logFilename, (Severity)logLevel);
    fileSink->setKeepNo(fileNum);
    fileSink->setSwitchDays(switchDays);

    sinksM.push_back(fileSink);
    bool runInBackground = g_app->runInBackground();
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
    meta->logTypeM = theLogger.logType();
    g_io_processor->PROCESS(0, &Logger::handleLogMeta, &theLogger, meta);
    return theLogger;
}


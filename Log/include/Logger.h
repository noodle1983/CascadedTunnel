#ifndef LOGGER_H
#define LOGGER_H

#include "LogStruct.h"
#include "Singleton.hpp"
#include "Sink.h"
#include <sstream>
#include <iostream>
#include <vector>

namespace nd
{
    const int LOG_TYPE_CFG = 0;
    const int LOG_TYPE_NORMAL = 1;
    extern thread_local ThreadLocalLogMeta tl_logmeta;
    class Logger
    {
    public:
        Logger(int logType);
        ~Logger();

        void initCfgLog();
        void initNormalLog();
        bool willAccept();

        void handleLogMeta(LogMeta* meta);

    private:
        int minSeverityM;
        std::vector<Sink*> sinksM;
    };

    template<typename T>
    Logger& operator<<(Logger& theLogger, const T& t) {
        tl_logmeta.streamM << t;
        return theLogger;
    }

    template<> Logger& operator<<(Logger& theLogger, const Severity& severity);
    template<> Logger& operator<<(Logger& theLogger, const Condition& condition);
    template<> Logger& operator<<(Logger& theLogger, const SrcLine& srcline);
    template<> Logger& operator<<(Logger& theLogger, const LogEnd& end);
    typedef DesignPattern::ParamSingleton<Logger, 0> CfgLoggerSingleton;
    typedef DesignPattern::ParamSingleton<Logger, 1> LoggerSingleton;
}

#define g_logger nd::LoggerSingleton::instance()
#define g_cfg_logger nd::CfgLoggerSingleton::instance()

#endif /* LOGGER_H */


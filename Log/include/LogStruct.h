#ifndef LOGSTRUCT_H
#define LOGSTRUCT_H

#include <sstream>
#include <string>
#include <chrono>
#include <assert.h>

namespace nd
{
    const int LOG_TYPE_CFG = 0;
    const int LOG_TYPE_NORMAL = 1;

    enum Severity
    {
        Trace = 0,
        Debug = 1,
        Info = 2, 
        Warning = 3, 
        Error = 4, 
        Fatal = 5, 
    };
    static inline const char* severityToStr(Severity severity)
    {
        switch (severity)
        {
            case Severity::Trace:
                return " TRACE ";
            case Severity::Debug:
                return " DEBUG ";
            case Severity::Info:
                return " INFO  ";
            case Severity::Warning:
                return " WARN  ";
            case Severity::Error:
                return " ERROR ";
            case Severity::Fatal:
                return " FATAL ";
        }
        return "";
    }


    struct Condition
    {
        Condition(bool isLog)
            :isLogM(isLog)
        {}
        bool isLogM;
    };

    struct LogEnd{};

    struct ThreadLocalLogMeta
    {
        void clear(){
            severityM = Severity::Debug; 
            ignoreM = false; 
            streamM.str("");
            streamM.clear();
        }
        Severity severityM = Severity::Debug; 
        bool ignoreM = false; 
        std::stringstream streamM;
    };

    using time_point_sys_clock = std::chrono::time_point<std::chrono::system_clock>;
    struct LogMeta
    {
        LogMeta() = default;
        LogMeta(const LogMeta& other) = delete;
        LogMeta(ThreadLocalLogMeta&& other)
            : timepointM(std::chrono::system_clock::now())
            , severityM(other.severityM)
            , streamM(std::move(other.streamM))
        {
            assert(other.ignoreM == false);

            other.severityM = Severity::Debug; 
            other.ignoreM = false; 
            other.streamM.clear();
        }

        time_point_sys_clock timepointM;
        Severity severityM; 
        int logTypeM;
        std::stringstream streamM;
    };
}

#endif /* LOGSTRUCT_H */


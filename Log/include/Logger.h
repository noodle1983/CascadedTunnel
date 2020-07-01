#ifndef LOGGER_H
#define LOGGER_H

#include "Singleton.hpp"

namespace Log
{
    class Logger
    {
    public:
        Logger();
        ~Logger();

        void init();

    private:
    };
    typedef DesignPattern::Singleton<Logger> CfgLoggerSingleton;
    typedef DesignPattern::InitDataSingleton<Logger> LoggerSingleton;
}

#define g_logger Log::LoggerSingleton::instance()
#define g_cfg_logger Log::CfgLoggerSingleton::instance()

#endif /* LOGGER_H */


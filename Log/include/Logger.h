#ifndef LOGGER_H
#define LOGGER_H

#include "Singleton.hpp"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>

namespace logging=boost::log;
namespace src = boost::log::sources;
using namespace logging::trivial;

namespace Log
{
    class Logger
    {
    public:
        Logger();
        ~Logger();

        void init();

        static src::severity_logger<severity_level> lg;
    private:
    };
    typedef DesignPattern::Singleton<Logger> CfgLoggerSingleton;
    typedef DesignPattern::InitDataSingleton<Logger> LoggerSingleton;
}

#define g_logger Log::LoggerSingleton::instance()
#define g_cfg_logger Log::CfgLoggerSingleton::instance()

#endif /* LOGGER_H */


#ifndef LOGGER_H
#define LOGGER_H

#include "Singleton.hpp"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

namespace logging=boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;

namespace Log
{
    class Logger
    {
    public:
        Logger();
        ~Logger();

        void init();

        src::severity_logger<severity_level> lg;
    private:
    };
    typedef DesignPattern::Singleton<Logger> LoggerSingleton;
}

#define g_logger Log::LoggerSingleton::instance()

#endif /* LOGGER_H */


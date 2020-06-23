#include "Logger.h"
#include "ConfigCenter.h"

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/stream.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/support/date_time.hpp>

namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

using namespace Log;
using namespace Config;
src::severity_logger<severity_level> Logger::lg;

//-----------------------------------------------------------------------------

Logger::Logger()
{
}

//-----------------------------------------------------------------------------

Logger::~Logger()
{
}

//-----------------------------------------------------------------------------

void Logger::init()
{
    //int logLevel = g_cfg->get("log.level", 3);
    bool runInBackground = g_cfg->get("process.background", 1);
    std::string logFilename = g_cfg->get("log.filename", "trouble_shooting_%N.log");
    int fileSize = g_cfg->get("log.fileSize", 10);
    //int fileNum = g_cfg->get("log.fileNum", 10);
    std::string logPattern = g_cfg->get("log.pattern", "%D{%y-%m-%d %H:%M:%S.%q} %-5p [%l] %m%n");

    if (runInBackground) {
        logging::add_file_log (
            keywords::file_name = logFilename,
            keywords::rotation_size = fileSize * 1024 * 1024,
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
            keywords::format = "[%TimeStamp%][%Severity%] %Message%"
        );
    }
    else {
        logging::add_console_log (
            std::cout,
            boost::parameter::keyword<keywords::tag::format>::get() = 
            ( 
                expr::stream 
                    << "["   << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                    << "][" << boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID")
                    << "] " << std::left << std::setw(7) << std::setfill(' ')  << boost::log::trivial::severity 
                    << " " << expr::smessage
            )
        );

    }

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::debug
    );

    logging::add_common_attributes();
}

//-----------------------------------------------------------------------------


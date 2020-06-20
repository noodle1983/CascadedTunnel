#include "Logger.h"
#include "ConfigCenter.h"

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>


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

    if (runInBackground)
    {
        logging::add_file_log
        (
            keywords::file_name = logFilename,
            keywords::rotation_size = fileSize * 1024 * 1024,
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
            keywords::format = "[%TimeStamp%] %Severity% %Message%"
        );
    }

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::debug
    );

    logging::add_common_attributes();
}

//-----------------------------------------------------------------------------


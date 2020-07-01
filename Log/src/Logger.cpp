#include "Logger.h"
#include "ConfigCenter.h"

using namespace Log;
using namespace Config;

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
    }
    else {

    }
}

//-----------------------------------------------------------------------------


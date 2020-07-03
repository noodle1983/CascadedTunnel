#ifndef LOG_H
#define LOG_H

#include "Logger.h"
#include <iostream>

//-----------------------------------------------------------------------------
//trafficLogger
#define LOG_TRACE(msg) \
*g_logger << nd::Severity::Trace \
    << nd::SrcLine(__FILE__, __LINE__) \
    << msg << nd::LogEnd()

#define LOG_DEBUG(msg) \
*g_logger << nd::Severity::Debug \
    << nd::SrcLine(__FILE__, __LINE__) \
    << msg << nd::LogEnd()

#define LOG_INFO(msg) \
*g_logger << nd::Severity::Info \
    << nd::SrcLine(__FILE__, __LINE__) \
    << msg << nd::LogEnd()

#define LOG_WARN(msg) \
*g_logger << nd::Severity::Warning \
    << nd::SrcLine(__FILE__, __LINE__) \
    << msg << nd::LogEnd()

#define LOG_ERROR(msg) \
*g_logger << nd::Severity::Error \
    << nd::SrcLine(__FILE__, __LINE__) \
    << msg << nd::LogEnd()

#define LOG_FATAL(msg) \
*g_logger << nd::Severity::Fatal \
    << nd::SrcLine(__FILE__, __LINE__) \
    << msg << nd::LogEnd()

//-----------------------------------------------------------------------------
//configLogger
#define CFG_TRACE(msg) \
std::cout \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg << std::endl

#define CFG_DEBUG(msg) \
std::cout \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg << std::endl

#define CFG_INFO(msg) \
std::cout \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg << std::endl

#define CFG_WARN(msg) \
std::cout \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg << std::endl

#define CFG_ERROR(msg) \
std::cout \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg << std::endl

#define CFG_FATAL(msg) \
std::cout \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg << std::endl

#endif /* LOG_H */

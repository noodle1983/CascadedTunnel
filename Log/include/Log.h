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
*g_cfg_logger << nd::Severity::Trace \
    << nd::SrcLine(__FILE__, __LINE__) \
    << msg << nd::LogEnd()

#define CFG_DEBUG(msg) \
*g_cfg_logger << nd::Severity::Debug \
    << nd::SrcLine(__FILE__, __LINE__) \
    << msg << nd::LogEnd()

#define CFG_INFO(msg) \
*g_cfg_logger << nd::Severity::Info \
    << nd::SrcLine(__FILE__, __LINE__) \
    << msg << nd::LogEnd()

#define CFG_WARN(msg) \
*g_cfg_logger << nd::Severity::Warning \
    << nd::SrcLine(__FILE__, __LINE__) \
    << msg << nd::LogEnd()

#define CFG_ERROR(msg) \
*g_cfg_logger << nd::Severity::Error \
    << nd::SrcLine(__FILE__, __LINE__) \
    << msg << nd::LogEnd()

#define CFG_FATAL(msg) \
*g_cfg_logger << nd::Severity::Fatal \
    << nd::SrcLine(__FILE__, __LINE__) \
    << msg << nd::LogEnd()


#endif /* LOG_H */

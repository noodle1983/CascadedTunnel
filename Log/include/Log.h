#ifndef LOG_H
#define LOG_H

#include "Logger.h"
#include <iostream>
#include <thread>

//-----------------------------------------------------------------------------
//trafficLogger
#define LOG_TRACE(msg) \
*g_logger << nd::Severity::Trace \
    << "[" << std::hex << std::this_thread::get_id() << std::dec << "] " \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define LOG_DEBUG(msg) \
*g_logger << nd::Severity::Debug \
    << "[" << std::hex << std::this_thread::get_id() << std::dec << "] " \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define LOG_INFO(msg) \
*g_logger << nd::Severity::Info \
    << "[" << std::hex << std::this_thread::get_id() << std::dec << "] " \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define LOG_WARN(msg) \
*g_logger << nd::Severity::Warning \
    << "[" << std::hex << std::this_thread::get_id() << std::dec << "] " \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define LOG_ERROR(msg) \
*g_logger << nd::Severity::Error \
    << "[" << std::hex << std::this_thread::get_id() << std::dec << "] " \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define LOG_FATAL(msg) \
*g_logger << nd::Severity::Fatal \
    << "[" << std::hex << std::this_thread::get_id() << std::dec << "] " \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

//-----------------------------------------------------------------------------
//sessionLogger
#define SE_TRACE(msg) \
*g_logger << nd::Severity::Trace \
    << "[" << std::hex << std::this_thread::get_id() << std::dec << ":" << sessionIdM << "] "  \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define SE_DEBUG(msg) \
*g_logger << nd::Severity::Debug \
    << "[" << std::hex << std::this_thread::get_id() << std::dec << ":" << sessionIdM << "] "  \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define SE_INFO(msg) \
*g_logger << nd::Severity::Info \
    << "[" << std::hex << std::this_thread::get_id() << std::dec << ":" << sessionIdM << "] "  \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define SE_WARN(msg) \
*g_logger << nd::Severity::Warning \
    << "[" << std::hex << std::this_thread::get_id() << std::dec << ":" << sessionIdM << "] "  \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define SE_ERROR(msg) \
*g_logger << nd::Severity::Error \
    << "[" << std::hex << std::this_thread::get_id() << std::dec << ":" << sessionIdM << "] "  \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define SE_FATAL(msg) \
*g_logger << nd::Severity::Fatal \
    << "[" << std::hex << std::this_thread::get_id() << std::dec << ":" << sessionIdM << "] "  \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

//-----------------------------------------------------------------------------
//configLogger
#define CFG_TRACE(msg) \
*g_cfg_logger << nd::Severity::Trace \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define CFG_DEBUG(msg) \
*g_cfg_logger << nd::Severity::Debug \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define CFG_INFO(msg) \
*g_cfg_logger << nd::Severity::Info \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define CFG_WARN(msg) \
*g_cfg_logger << nd::Severity::Warning \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define CFG_ERROR(msg) \
*g_cfg_logger << nd::Severity::Error \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()

#define CFG_FATAL(msg) \
*g_cfg_logger << nd::Severity::Fatal \
    << "(" << __FILE__ << ":" << __LINE__ << ")"  \
    << msg << nd::LogEnd()


#endif /* LOG_H */

#ifndef LOG_H
#define LOG_H

#include "Logger.h"

//-----------------------------------------------------------------------------
//trafficLogger
#define LOG_TRACE(msg) \
BOOST_LOG_SEV(g_logger->lg, trace)<<msg

#define LOG_DEBUG(msg) \
BOOST_LOG_SEV(g_logger->lg, debug)<<msg

#define LOG_INFO(msg) \
BOOST_LOG_SEV(g_logger->lg, info)<<msg

#define LOG_WARN(msg) \
BOOST_LOG_SEV(g_logger->lg, warning)<<msg

#define LOG_ERROR(msg) \
BOOST_LOG_SEV(g_logger->lg, error)<<msg

#define LOG_FATAL(msg) \
BOOST_LOG_SEV(g_logger->lg, fatal)<<msg

//-----------------------------------------------------------------------------
//configLogger
#define CFG_TRACE(msg) \
BOOST_LOG_SEV(g_cfg_logger->lg, trace)<<msg

#define CFG_DEBUG(msg) \
BOOST_LOG_SEV(g_cfg_logger->lg, debug)<<msg

#define CFG_INFO(msg) \
BOOST_LOG_SEV(g_cfg_logger->lg, info)<<msg

#define CFG_WARN(msg) \
BOOST_LOG_SEV(g_cfg_logger->lg, warning)<<msg

#define CFG_ERROR(msg) \
BOOST_LOG_SEV(g_cfg_logger->lg, error)<<msg

#define CFG_FATAL(msg) \
BOOST_LOG_SEV(g_cfg_logger->lg, fatal)<<msg
#endif /* LOG_H */

#ifndef LOG_H
#define LOG_H

#include "Logger.h"
#include <boost/thread/thread.hpp>

//-----------------------------------------------------------------------------
//trafficLogger
#define LOG_TRACE(msg) \
BOOST_LOG_SEV(g_logger->lg, trace) \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg

#define LOG_DEBUG(msg) \
BOOST_LOG_SEV(g_logger->lg, debug)\
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg

#define LOG_INFO(msg) \
BOOST_LOG_SEV(g_logger->lg, info) \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg

#define LOG_WARN(msg) \
BOOST_LOG_SEV(g_logger->lg, warning) \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg

#define LOG_ERROR(msg) \
BOOST_LOG_SEV(g_logger->lg, error) \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg

#define LOG_FATAL(msg) \
BOOST_LOG_SEV(g_logger->lg, fatal) \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg

//-----------------------------------------------------------------------------
//configLogger
#define CFG_TRACE(msg) \
BOOST_LOG_SEV(g_cfg_logger->lg, trace) \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg

#define CFG_DEBUG(msg) \
BOOST_LOG_SEV(g_cfg_logger->lg, debug) \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg

#define CFG_INFO(msg) \
BOOST_LOG_SEV(g_cfg_logger->lg, info) \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg

#define CFG_WARN(msg) \
BOOST_LOG_SEV(g_cfg_logger->lg, warning) \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg

#define CFG_ERROR(msg) \
BOOST_LOG_SEV(g_cfg_logger->lg, error) \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg

#define CFG_FATAL(msg) \
BOOST_LOG_SEV(g_cfg_logger->lg, fatal) \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg

#endif /* LOG_H */

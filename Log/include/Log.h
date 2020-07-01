#ifndef LOG_H
#define LOG_H

#include <iostream>

//-----------------------------------------------------------------------------
//trafficLogger
#define LOG_TRACE(msg) \
std::cout \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg << std::endl

#define LOG_DEBUG(msg) \
std::cout\
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg << std::endl

#define LOG_INFO(msg) \
std::cout \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg << std::endl

#define LOG_WARN(msg) \
std::cout \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg << std::endl

#define LOG_ERROR(msg) \
std::cout \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg << std::endl

#define LOG_FATAL(msg) \
std::cout \
    << "(" <<  __FILE__ << ":" << __LINE__ << ") "  \
    << msg << std::endl

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

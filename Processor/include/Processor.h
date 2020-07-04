#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "CppProcessor.h"

#define NEW_JOB(...) {new nd::Job(std::bind(__VA_ARGS__))}
#define PROCESS(fd, ...) process(fd, NEW_JOB(__VA_ARGS__))

#define g_net_processor nd::CppProcessor::netInstance()
#define g_fsm_processor nd::CppProcessor::fsmInstance()
#define g_man_processor nd::CppProcessor::manInstance()
#define g_io_processor nd::CppProcessor::ioInstance()

#endif /* PROCESSOR_H */


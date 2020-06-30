#ifndef BOOSTPROCESSOR_H
#define BOOSTPROCESSOR_H

#include "BoostWorker.h"

#include <string>
#include <thread>
#include <vector>

namespace Net
{
namespace Protocol
{
	class ProcessorSensor;
}
}

namespace Processor
{
    class BoostWorker;

    class BoostProcessor
    {
    public:
		friend class Net::Protocol::ProcessorSensor;

        BoostProcessor(const unsigned theThreadCount);
        BoostProcessor(const std::string& theName, const unsigned theThreadCount);
        ~BoostProcessor();

        static BoostProcessor* fsmInstance();
        static BoostProcessor* netInstance();
        static BoostProcessor* manInstance();
        static BoostProcessor* ioInstance();

        void start();
        void waitStop();
        void stop();

		min_heap_item_t* addLocalTimer(
                const unsigned long long theId, 
				const struct timeval& theInterval, 
				TimerCallback theCallback,
				void* theArg)
        {
            unsigned workerId = theId % threadCountM;
            return workersM[workerId].addLocalTimer(theInterval, theCallback, theArg);
        }
		inline void cancelLocalTimer(
                const unsigned long long theId, 
                min_heap_item_t*& theEvent)
        {
            unsigned workerId = theId % threadCountM;
            return workersM[workerId].cancelLocalTimer(theEvent);
        }

        void process(
                const unsigned long long theId, 
                Job* theJob)
        {
            unsigned workerId = theId % threadCountM;
            workersM[workerId].process(theJob);
        }

    private:
        unsigned threadCountM;
        BoostWorker* workersM;
        std::vector<std::thread> threadsM;
        std::string nameM;

        static BoostProcessor* fsmProcessorM;
        static BoostProcessor* netProcessorM;
        static BoostProcessor* manProcessorM;
        static BoostProcessor* ioProcessorM;
    };
}
#define g_net_processor Processor::BoostProcessor::netInstance()
#define g_fsm_processor Processor::BoostProcessor::fsmInstance()
#define g_io_processor Processor::BoostProcessor::ioInstance()

#endif /* BOOSTPROCESSOR_H */


#ifndef CPPPROCESSOR_H
#define CPPPROCESSOR_H

#include "CppWorker.h"

#include <string>
#include <thread>
#include <vector>

namespace nd
{
	class ProcessorSensor;
    class CppWorker;

    class CppProcessor
    {
    public:
		friend class nd::ProcessorSensor;

        CppProcessor(const unsigned theThreadCount);
        CppProcessor(const std::string& theName, const unsigned theThreadCount);
        ~CppProcessor();

        static CppProcessor* fsmInstance();
        static CppProcessor* netInstance();
        static CppProcessor* manInstance();
        static CppProcessor* ioInstance();

        void start(bool toWaitStop = false);
        // must not call stop in its worker
        void waitStop();
        void stop();

		min_heap_item_t* addLocalTimer(
                const unsigned long long theId, 
				const struct timeval& theInterval, 
				TimerCallback theCallback,
				void* theArg)
        {
            if (NULL == workersM) {return NULL;}
            unsigned workerId = theId % threadCountM;
            return workersM[workerId].addLocalTimer(theInterval, theCallback, theArg);
        }
		inline void cancelLocalTimer(
                const unsigned long long theId, 
                min_heap_item_t*& theEvent)
        {
            if (NULL == workersM) {return;}
            unsigned workerId = theId % threadCountM;
            return workersM[workerId].cancelLocalTimer(theEvent);
        }

        void process(
                const unsigned long long theId, 
                Job* theJob)
        {
            if (NULL == workersM) {return;}
            unsigned workerId = theId % threadCountM;
            workersM[workerId].process(theJob);
        }

    private:
        unsigned threadCountM;
        CppWorker* workersM;
        std::vector<std::thread> threadsM;
        std::string nameM;
        bool waitStopM; 
        std::mutex stopMutexM;

        static CppProcessor* fsmProcessorM;
        static CppProcessor* netProcessorM;
        static CppProcessor* manProcessorM;
        static CppProcessor* ioProcessorM;
    };
}

#endif /* CPPPROCESSOR_H */


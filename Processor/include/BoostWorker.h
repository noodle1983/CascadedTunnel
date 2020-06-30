#ifndef BOOSTWORKER_H
#define BOOSTWORKER_H

#include "ProcessorJob.hpp"

#include <functional>
#include <condition_variable>
#include <list>

#include <min_heap.h>

typedef void (*TimerCallback)(void *arg);
#define NEW_JOB(...) {new Processor::Job(std::bind(__VA_ARGS__))}

namespace Processor
{
    typedef std::function<void ()> Job;
    typedef std::list<Job*> JobQueue;
    class BoostWorker
    {
    public:
        BoostWorker();
        ~BoostWorker();
        void setGroupInfo(const unsigned theTotal, const unsigned theIndex)
        {
            groupTotalM = theTotal;
            groupIndexM = theIndex;
        }

        inline bool isJobQueueEmpty()
        {
            std::lock_guard<std::mutex> lock(queueMutexM);
            return jobQueueM.empty();
        }
		inline unsigned getQueueSize()
		{
            std::lock_guard<std::mutex> lock(queueMutexM);
            return jobQueueM.size();
        }
        void stop();

        void process(Job* theJob);
		min_heap_item_t* addLocalTimer(
				const struct timeval& theInterval, 
				TimerCallback theCallback,
				void* theArg);
		void cancelLocalTimer(min_heap_item_t*& theEvent);

        void run();

        //for timer only
        void initThreadAttr();
		void handleLocalTimer();
        // and addLocalTimer cancelLocalTimer

    private:
        unsigned groupTotalM;
        unsigned groupIndexM;
		
        JobQueue jobQueueM;
        std::mutex queueMutexM;
        std::mutex nullMutexM;
        std::condition_variable queueCondM;

		//integrate timer handling
		min_heap_t timerHeapM;
		struct timeval timeNowM;	

        mutable bool isToStopM;
#ifdef DEBUG
		int64_t tidM;
#endif
    };
}

#endif /* BOOSTWORKER_H */


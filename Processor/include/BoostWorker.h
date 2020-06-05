#ifndef BOOSTWORKER_H
#define BOOSTWORKER_H

#include "ProcessorJob.hpp"
#include "KfifoBuffer.h"

#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <list>

#include <min_heap.h>

typedef void (*TimerCallback)(void *arg);

namespace Processor
{
    //typedef std::list<IJob*> JobQueue;
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
        {return bufferJobQueueM.empty();}
		inline unsigned getQueueSize()
		{return bufferJobQueueM.size()/sizeof(void*);}
        void stop();

        int process(IJob* theJob);
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
		
        //JobQueue jobQueueM;
		Utility::KfifoBuffer bufferJobQueueM;
        boost::mutex queueMutexM;
        boost::mutex nullMutexM;
        boost::condition_variable queueCondM;

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


#include "CppWorker.h"
#include "Log.h"

#include <event2/util.h>
#include <assert.h>

using namespace nd;
using namespace std;

thread_local unsigned g_threadGroupTotal = 0;
thread_local unsigned g_threadGroupIndex = 0;

#ifdef DEBUG
#include <thread>
static const thread::id default_thread_id;
#endif
//-----------------------------------------------------------------------------
CppWorker::CppWorker()
    : groupTotalM(0)
    , groupIndexM(-1)
    , isToStopM(false)
    , isWaitStopM(false)
{
    min_heap_ctor(&timerHeapM);	
    timeNowM.tv_sec = 0;
    timeNowM.tv_usec = 0;
}

//-----------------------------------------------------------------------------

CppWorker::~CppWorker()
{
    min_heap_dtor(&timerHeapM);	
}

//-----------------------------------------------------------------------------

void CppWorker::stop()
{
    isToStopM = true;
    queueCondM.notify_one();
}

//-----------------------------------------------------------------------------

void CppWorker::waitStop()
{
    isWaitStopM = true;
    queueCondM.notify_one();
}

//-----------------------------------------------------------------------------


void CppWorker::process(Job* theJob)
{
    bool jobQueueEmpty = false;
    {
        lock_guard<mutex> lock(queueMutexM);
        jobQueueEmpty = jobQueueM.empty();
        jobQueueM.push_back(theJob);
    }
    if (jobQueueEmpty)
    {
        queueCondM.notify_one();
    }
}

//-----------------------------------------------------------------------------

min_heap_item_t* CppWorker::addLocalTimer(
        const struct timeval& theInterval, 
		TimerCallback theCallback,
		void* theArg)
{
#ifdef DEBUG
    unsigned threadCount = g_threadGroupTotal;
    unsigned threadIndex = g_threadGroupIndex;
    if (threadIndex != groupIndexM || threadCount != groupTotalM)
    {
        LOG_FATAL("job is handled in wrong thread:" << threadIndex 
                << ", count:" << threadCount
                << ", worker's index:" << groupIndexM
                << ", worker's groupCount:" << groupTotalM);
        assert(false);
    }
    

    if (default_thread_id == tidM)
    {
        tidM = this_thread::get_id();
    }
    else if (tidM != this_thread::get_id())
    {
        LOG_FATAL("tid not match pre:" << tidM << "-now:" << this_thread::get_id());
        assert(false);
    }
#endif

	bool timerHeapEmpty = false;
    //if it is the first one, get the time of now
    //else reuse the one in run()
    if ((timerHeapEmpty = min_heap_empty(&timerHeapM)))
    {
        evutil_gettimeofday(&timeNowM, NULL);
    }
    if (1024 > min_heap_size(&timerHeapM))
    {
        min_heap_reserve(&timerHeapM, 1024);
    }

	min_heap_item_t* timeoutEvt = new min_heap_item_t();
    timeoutEvt->callback = theCallback;
    timeoutEvt->arg = theArg;
    timeoutEvt->timeout.tv_usec = timeNowM.tv_usec + theInterval.tv_usec;
    timeoutEvt->timeout.tv_sec = timeNowM.tv_sec 
        + theInterval.tv_sec 
        + timeoutEvt->timeout.tv_usec/1000000;
    timeoutEvt->timeout.tv_usec %= 1000000;

    if (-1 == min_heap_push(&timerHeapM, timeoutEvt))
    {
        LOG_FATAL("not enough memory!");
        exit(-1);
    }
	if (timerHeapEmpty)
	{
        queueCondM.notify_one();
	}
	return timeoutEvt;
}

//-----------------------------------------------------------------------------

void CppWorker::cancelLocalTimer(min_heap_item_t*& theEvent)
{
    min_heap_erase(&timerHeapM, theEvent);
    delete theEvent;
	theEvent = NULL;
}

//-----------------------------------------------------------------------------

void CppWorker::initThreadAttr()
{
    g_threadGroupTotal = groupTotalM;
    g_threadGroupIndex = groupIndexM;
}

//-----------------------------------------------------------------------------

void CppWorker::handleLocalTimer()
{
    if (!min_heap_empty(&timerHeapM))
    {
        evutil_gettimeofday(&timeNowM, NULL);
        while(!min_heap_empty(&timerHeapM)) 
        {
            min_heap_item_t* topEvent = min_heap_top(&timerHeapM);
            if (item_cmp(&topEvent->timeout, &timeNowM, <=))
            {
                min_heap_pop(&timerHeapM);
                (topEvent->callback)(topEvent->arg);
                delete topEvent;
            }
            else
            {
                break;
            }
        } 
    }


}

//-----------------------------------------------------------------------------

void CppWorker::run()
{
    initThreadAttr();
    Job* job;
    while (!isToStopM)
    {
        job = NULL;
        {
            lock_guard<mutex> lock(queueMutexM);
            if (!jobQueueM.empty())
            {
                job = jobQueueM.front();
                jobQueueM.pop_front();
            }
            else if(isWaitStopM)
            {
                break;
            }
        }

        //handle Job
        if (job != NULL)
        {
            (*job)();
        }

        //handle timer
        handleLocalTimer();

        unique_lock<mutex> queueLock(queueMutexM);
		if (!jobQueueM.empty()) { continue; }

        if (!isToStopM && !isWaitStopM && jobQueueM.empty() && !min_heap_empty(&timerHeapM))
        {
            queueCondM.wait_for(queueLock, chrono::microseconds(500));
        }
        else
        {
            while (!isToStopM && !isWaitStopM && jobQueueM.empty() && min_heap_empty(&timerHeapM))
            {
                queueCondM.wait(queueLock); 
            }
        }
    }
}


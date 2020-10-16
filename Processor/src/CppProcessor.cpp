#include "CppProcessor.h"
#include "CppWorker.h"
#include "ConfigCenter.h"

#include <functional>
#include <chrono>

using namespace std;
using namespace nd;

//-----------------------------------------------------------------------------

static mutex fsmProcessorInstanceMutex;
static mutex netProcessorInstanceMutex;
static mutex manProcessorInstanceMutex;
static mutex ioProcessorInstanceMutex;
static shared_ptr<CppProcessor> fsmProcessorInstanceReleaser;
static shared_ptr<CppProcessor> netProcessorInstanceReleaser;
static shared_ptr<CppProcessor> manProcessorInstanceReleaser;
static shared_ptr<CppProcessor> ioProcessorInstanceReleaser;
CppProcessor* CppProcessor::fsmProcessorM = NULL;
CppProcessor* CppProcessor::netProcessorM = NULL;
CppProcessor* CppProcessor::manProcessorM = NULL;
CppProcessor* CppProcessor::ioProcessorM = NULL;
//-----------------------------------------------------------------------------

CppProcessor* CppProcessor::fsmInstance()
{
    if (NULL == fsmProcessorM)
    {
        lock_guard<mutex> lock(fsmProcessorInstanceMutex);
        if (NULL == fsmProcessorM)
        {
            int threadCount = g_cfg->get("prc.fsmTno", 3);
            CppProcessor* fsmProcessor = new CppProcessor(threadCount);
            fsmProcessorInstanceReleaser.reset(fsmProcessor);
            fsmProcessor->start();
            fsmProcessorM = fsmProcessor;
        }
    }
    return fsmProcessorM;
}


//-----------------------------------------------------------------------------

CppProcessor* CppProcessor::netInstance()
{
    if (NULL == netProcessorM)
    {
        lock_guard<mutex> lock(netProcessorInstanceMutex);
        if (NULL == netProcessorM)
        {
            int threadCount = g_cfg->get("prc.netTno", 3);
            CppProcessor* netProcessor = new CppProcessor(threadCount);
            netProcessorInstanceReleaser.reset(netProcessor);
            netProcessor->start();
            netProcessorM = netProcessor;
        }
    }
    return netProcessorM;
}

//-----------------------------------------------------------------------------

CppProcessor* CppProcessor::manInstance()
{
    if (NULL == manProcessorM)
    {
        lock_guard<mutex> lock(manProcessorInstanceMutex);
        if (NULL == manProcessorM)
        {
            int threadCount = g_cfg->get("prc.manTno", 1);
            CppProcessor* manProcessor = new CppProcessor(threadCount);
            manProcessorInstanceReleaser.reset(manProcessor);
            manProcessor->start();
            manProcessorM = manProcessor;
        }
    }
    return manProcessorM;
}

//-----------------------------------------------------------------------------

CppProcessor* CppProcessor::ioInstance()
{
    if (NULL == ioProcessorM)
    {
        lock_guard<mutex> lock(ioProcessorInstanceMutex);
        if (NULL == ioProcessorM)
        {
            int threadCount = 1; 
            CppProcessor* ioProcessor = new CppProcessor(threadCount);
            ioProcessorInstanceReleaser.reset(ioProcessor);
            ioProcessor->start(true);
            ioProcessorM = ioProcessor;
        }
    }
    return ioProcessorM;
}

//-----------------------------------------------------------------------------

CppProcessor::CppProcessor(const unsigned theThreadCount)
    : threadCountM(theThreadCount)
    , workersM(NULL)
{
}

//-----------------------------------------------------------------------------

CppProcessor::CppProcessor(const std::string& theName, const unsigned theThreadCount)
    : threadCountM(theThreadCount)
    , workersM(NULL)
    , nameM(theName)
    , waitStopM(false)
{
}

//-----------------------------------------------------------------------------

CppProcessor::~CppProcessor()
{
    if (workersM) {
        if (waitStopM){
            waitStop();
        }
        else {
            stop();
        }
    }
}

//-----------------------------------------------------------------------------

void CppProcessor::start(bool toWaitStop)
{
    waitStopM = toWaitStop;
    if (0 == threadCountM)
        return;

    if (NULL != workersM)
        return;

    workersM = new CppWorker[threadCountM];
    threadsM.reserve(threadCountM);
    for (unsigned i = 0; i < threadCountM; i++)
    {
        workersM[i].setGroupInfo(threadCountM, i);
        threadsM.push_back(thread(&CppWorker::run, &workersM[i]));
    }
}

//-----------------------------------------------------------------------------

void CppProcessor::waitStop()
{
    lock_guard<mutex> lock(stopMutexM);
    if (NULL == workersM)
        return;

    unsigned int i = 0;
    while(true)
    {
        /* check the worker once only */
        if(i < threadCountM && workersM[i].isJobQueueEmpty())
        {
            workersM[i].waitStop();
            i++;
        }
        if (i == threadCountM)
        {
            break;
        }
        else
        {
            this_thread::sleep_for(chrono::milliseconds(1));
        }
    }
    for (unsigned i = 0; i < threadCountM; i++)
    {
        threadsM[i].join();
    }
    delete []workersM;
    workersM = NULL;
}

//-----------------------------------------------------------------------------

void CppProcessor::stop()
{
    lock_guard<mutex> lock(stopMutexM);
    if (NULL == workersM)
        return;

    for (unsigned i = 0; i < threadCountM; i++)
    {
        workersM[i].stop();
    }
    for (unsigned i = 0; i < threadCountM; i++)
    {
        threadsM[i].join();
    }
    delete []workersM;
    workersM = NULL;
}

//-----------------------------------------------------------------------------

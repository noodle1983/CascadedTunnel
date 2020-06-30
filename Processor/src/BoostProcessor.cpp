#include "BoostProcessor.h"
#include "BoostWorker.h"
#include "ConfigCenter.h"
#include "ProcessorJob.hpp"
#include "ProcessorSensor.h"

#include <functional>

using namespace std;
using namespace Processor;
using namespace Config;

//-----------------------------------------------------------------------------

static mutex fsmProcessorInstanceMutex;
static mutex netProcessorInstanceMutex;
static mutex manProcessorInstanceMutex;
static mutex ioProcessorInstanceMutex;
static shared_ptr<BoostProcessor> fsmProcessorInstanceReleaser;
static shared_ptr<BoostProcessor> netProcessorInstanceReleaser;
static shared_ptr<BoostProcessor> manProcessorInstanceReleaser;
static shared_ptr<BoostProcessor> ioProcessorInstanceReleaser;
BoostProcessor* BoostProcessor::fsmProcessorM = NULL;
BoostProcessor* BoostProcessor::netProcessorM = NULL;
BoostProcessor* BoostProcessor::manProcessorM = NULL;
BoostProcessor* BoostProcessor::ioProcessorM = NULL;
//-----------------------------------------------------------------------------

BoostProcessor* BoostProcessor::fsmInstance()
{
    if (NULL == fsmProcessorM)
    {
        lock_guard<mutex> lock(fsmProcessorInstanceMutex);
        if (NULL == fsmProcessorM)
        {
            int threadCount = ConfigCenter::instance()->get("prc.fsmTno", 3);
            BoostProcessor* fsmProcessor = new BoostProcessor(threadCount);
            fsmProcessorInstanceReleaser.reset(fsmProcessor);
            fsmProcessor->start();
            fsmProcessorM = fsmProcessor;
        }
    }
    return fsmProcessorM;
}


//-----------------------------------------------------------------------------

BoostProcessor* BoostProcessor::netInstance()
{
    if (NULL == netProcessorM)
    {
        lock_guard<mutex> lock(netProcessorInstanceMutex);
        if (NULL == netProcessorM)
        {
            int threadCount = ConfigCenter::instance()->get("prc.netTno", 3);
            BoostProcessor* netProcessor = new BoostProcessor(threadCount);
            netProcessorInstanceReleaser.reset(netProcessor);
            netProcessor->start();
            netProcessorM = netProcessor;
        }
    }
    return netProcessorM;
}

//-----------------------------------------------------------------------------

BoostProcessor* BoostProcessor::manInstance()
{
    if (NULL == manProcessorM)
    {
        lock_guard<mutex> lock(manProcessorInstanceMutex);
        if (NULL == manProcessorM)
        {
            int threadCount = ConfigCenter::instance()->get("prc.manTno", 1);
            BoostProcessor* manProcessor = new BoostProcessor(threadCount);
            manProcessorInstanceReleaser.reset(manProcessor);
            manProcessor->start();
            manProcessorM = manProcessor;
        }
    }
    return manProcessorM;
}

//-----------------------------------------------------------------------------

BoostProcessor* BoostProcessor::ioInstance()
{
    if (NULL == ioProcessorM)
    {
        lock_guard<mutex> lock(ioProcessorInstanceMutex);
        if (NULL == ioProcessorM)
        {
            int threadCount = ConfigCenter::instance()->get("prc.ioTno", 1);
            BoostProcessor* ioProcessor = new BoostProcessor(threadCount);
            ioProcessorInstanceReleaser.reset(ioProcessor);
            ioProcessor->start();
            ioProcessorM = ioProcessor;
        }
    }
    return ioProcessorM;
}

//-----------------------------------------------------------------------------

BoostProcessor::BoostProcessor(const unsigned theThreadCount)
    : threadCountM(theThreadCount)
    , workersM(NULL)
{
}

//-----------------------------------------------------------------------------

BoostProcessor::BoostProcessor(const std::string& theName, const unsigned theThreadCount)
    : threadCountM(theThreadCount)
    , workersM(NULL)
    , nameM(theName)
{
}

//-----------------------------------------------------------------------------

BoostProcessor::~BoostProcessor()
{
    if (workersM)
    {
        stop();
    }
}

//-----------------------------------------------------------------------------

void BoostProcessor::start()
{
    if (0 == threadCountM)
        return;

    if (NULL != workersM)
        return;

    workersM = new BoostWorker[threadCountM];
    threadsM.reserve(threadCountM);
    for (unsigned i = 0; i < threadCountM; i++)
    {
        workersM[i].setGroupInfo(threadCountM, i);
        threadsM.push_back(thread(&BoostWorker::run, &workersM[i]));
    }
}

//-----------------------------------------------------------------------------

void BoostProcessor::waitStop()
{
    if (NULL == workersM)
        return;

    unsigned int i = 0;
    while(true)
    {
        /* check the worker once only */
        if(i < threadCountM && workersM[i].isJobQueueEmpty())
        {
            workersM[i].stop();
            i++;
        }
        if (i == threadCountM)
        {
            break;
        }
        else
        {
            usleep(1);
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

void BoostProcessor::stop()
{
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

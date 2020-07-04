#include "ProcessorSensor.h"
#include "TelnetCmdManager.h"

#include <sstream>
#include <iomanip>

using namespace nd;
using namespace std;

//-----------------------------------------------------------------------------

ProcessorSensorData::ProcessorSensorData()
	: telnetManagerM(NULL)
	, statCountM(0)
	, intervalM(3)
	, sensorM(NULL)
{}

//-----------------------------------------------------------------------------

ProcessorSensorData::~ProcessorSensorData()
{
	if (timeoutEvtM)
	{
		telnetManagerM->cancelLocalTimer(timeoutEvtM);
	}
}

//-----------------------------------------------------------------------------

ProcessorSensor::ProcessorSensor()
{
}

//-----------------------------------------------------------------------------

ProcessorSensor::~ProcessorSensor()
{

}

//-----------------------------------------------------------------------------

void ProcessorSensor::handle(TelnetCmdManager* theManager, CmdArgsList& theArgs)
{
    ProcessorSensorData* data = (ProcessorSensorData*)theManager->takeOverInputHandler(this);
    data->telnetManagerM = theManager;
    data->statCountM = 0;
    data->intervalM = 3;
    data->timeoutEvtM = NULL;
    data->sensorM = this;
    stat(data);
}

//-----------------------------------------------------------------------------

void ProcessorSensor::handle(
        TelnetCmdManager* theManager, 
        CmdArgsList& theArgs, 
        void* theSessionData)
{
    ProcessorSensorData* data = (ProcessorSensorData*) theSessionData;
    if (data->timeoutEvtM)
    {
        theManager->cancelLocalTimer(data->timeoutEvtM);
        data->timeoutEvtM = NULL;
    }
    data->telnetManagerM = NULL;
    data->statCountM = 0;
    data->intervalM = 3;
    theManager->exitCurCmd();
}

//-----------------------------------------------------------------------------

void ProcessorSensor::stat(ProcessorSensorData* theData)
{
    assert(theData != NULL);
	std::ostringstream oss;
    oss << "-------------------------" << "\r\n";
    oss << std::setiosflags(std::ios_base::left) 
        << std::setw(15) << "ProcessorName" 
        << std::setw(10) << "QueueSize" << "\r\n";
	{
        lock_guard<mutex> lock(processorMapMutexM);
		ProcessorMap::iterator it = processorMapM.begin();
		for (; it != processorMapM.end(); it++)
		{
			const std::string& name = it->first;
			CppProcessor* processor = it->second;
			for (unsigned i = 0; i < processor->threadCountM; i++)
			{
				std::ostringstream ossName;
				ossName << name << i;
				oss << std::setiosflags(std::ios_base::left) 
				    << std::setw(15) << ossName.str() 
					<< std::setw(10) << processor->workersM[i].getQueueSize() << "\r\n";
			}
		}
	}
	std::string str = oss.str();
	theData->telnetManagerM->send(str.c_str(), str.length());

	theData->statCountM++;
	addTimer(theData);
}

//-----------------------------------------------------------------------------

static void onProcessorSensorTimeOut(void *theArg)
{
    ProcessorSensorData* data = (ProcessorSensorData*)theArg;
	data->timeoutEvtM = NULL;
    data->sensorM->stat(data);
}

void ProcessorSensor::addTimer(ProcessorSensorData* theData)
{
	if (theData->timeoutEvtM)
	{
		theData->telnetManagerM->cancelLocalTimer(theData->timeoutEvtM);
		theData->timeoutEvtM = NULL;
	}
    struct timeval tv;
    tv.tv_sec = theData->intervalM;
    tv.tv_usec = 0;
    theData->timeoutEvtM = theData->telnetManagerM->addLocalTimer(tv, onProcessorSensorTimeOut, theData);
}

//-----------------------------------------------------------------------------

void ProcessorSensor::registProcessor(
		const std::string& theName, 
		CppProcessor* theProcessor)
{
	lock_guard<mutex> lock(processorMapMutexM);
	processorMapM[theName] = theProcessor;
}

//-----------------------------------------------------------------------------

void ProcessorSensor::unregistProcessor(
		const std::string& theName)
{
	lock_guard<mutex> lock(processorMapMutexM);
	processorMapM.erase(theName);
}

//-----------------------------------------------------------------------------


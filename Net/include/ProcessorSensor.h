#ifndef PROCESSORSENSOR_H
#define PROCESSORSENSOR_H

#include "ICmdHandler.h"
#include "Singleton.hpp"
#include "Processor.h"

#include <map>
#include <string>
#include <thread>

namespace nd
{
    class ProcessorSensor;
    struct ProcessorSensorData
    {
        ProcessorSensorData();
        ~ProcessorSensorData();
		min_heap_item_t* timeoutEvtM;
		TelnetCmdManager* telnetManagerM;
		int statCountM;
		int intervalM;
        ProcessorSensor* sensorM;
    };

    class ProcessorSensor: public ICmdHandler
    {
	public:
		typedef std::map<std::string, CppProcessor*> ProcessorMap;

		ProcessorSensor();
		virtual ~ProcessorSensor();
        //////////////////desc intent///////////////////////////////////////|
		virtual const char* getDesc() {return "prcstat                       show processor info.";};
		virtual const char* getPrompt(){return "";};
		virtual void handle(TelnetCmdManager* theManager, CmdArgsList& theArgs);

        virtual void* newCmdSessionData(){return new ProcessorSensorData;}
        virtual void  freeCmdSessionData(void* theData){delete (ProcessorSensorData*)theData;} 
		virtual void handle(TelnetCmdManager* theManager, CmdArgsList& theArgs, void* theSessionData);

		void stat(ProcessorSensorData* theData);
		void addTimer(ProcessorSensorData* theData);

		void registProcessor(
				const std::string& theName, 
				CppProcessor* theProcessor);
		void unregistProcessor(
				const std::string& theName);

	private:
		ProcessorMap processorMapM;
        std::mutex processorMapMutexM;
    };
    typedef class DesignPattern::Singleton<ProcessorSensor> ProcessorSensorSingleton; 
}

#endif /* PROCESSORSENSOR_H */


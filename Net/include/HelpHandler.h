#ifndef HELPHANDLER_H
#define HELPHANDLER_H

#include "ICmdHandler.h"

namespace nd
{
    class HelpHandler: public ICmdHandler
    {
	public:
		HelpHandler(){}
		virtual ~HelpHandler(){}
        //////////////////desc intent///////////////////////////////////////|
		virtual const char* getDesc() {return "help/h                        print this message.";};
		virtual const char* getPrompt(){return "";};
		virtual void handle(TelnetCmdManager* theManager, CmdArgsList& theArgs);

    };
}

#endif


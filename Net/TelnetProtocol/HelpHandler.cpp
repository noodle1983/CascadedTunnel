#include "HelpHandler.h"
#include "TelnetCmdManager.h"

using namespace nd;
//-----------------------------------------------------------------------------

void HelpHandler::handle(TelnetCmdManager* theManager, CmdArgsList& theArgs)
{
    theManager->printUsage();
}


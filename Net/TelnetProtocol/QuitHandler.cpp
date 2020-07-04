#include "QuitHandler.h"
#include "TelnetCmdManager.h"

using namespace nd;
//-----------------------------------------------------------------------------

void QuitHandler::handle(TelnetCmdManager* theManager, CmdArgsList& theArgs)
{
    theManager->exitCurCmd();
}


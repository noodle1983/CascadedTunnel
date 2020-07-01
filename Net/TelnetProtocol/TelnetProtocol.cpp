#include "TelnetProtocol.h"
#include "ConfigCenter.h"
#include "SocketConnection.h"
#include "Log.h"

using namespace Net;
using namespace Net::Protocol;
using namespace Config;
using namespace std;


//-----------------------------------------------------------------------------

TelnetProtocol::TelnetProtocol(Processor::BoostProcessor* theProcessor)
	:IProtocol(theProcessor)
{
}

//-----------------------------------------------------------------------------

TelnetProtocol::~TelnetProtocol()
{
}

//-----------------------------------------------------------------------------

void TelnetProtocol::handleInput(Connection::SocketConnectionPtr theConnection)
{
	TelnetCmdManager* cmdManager = NULL;
    int fd = theConnection->getFd();
    {
        lock_guard<mutex> lock(manMapMutexM);
        Con2CmdManagerMap::iterator it = con2CmdManagerMapM.find(fd);
        if (it == con2CmdManagerMapM.end())
        {
            cmdManager = new TelnetCmdManager(theConnection->getPeerAddr(), theConnection, this);		
            con2CmdManagerMapM[fd] = cmdManager;
        }
        else if (!it->second->validate(theConnection->getPeerAddr()))
        {
            delete it->second;
            cmdManager = new TelnetCmdManager(theConnection->getPeerAddr(), theConnection, this);		
            it->second = cmdManager;
        }
        else
        {
            cmdManager = it->second;
        }
    }

    // the same fd's message should be handled in the same thread
    // so no lock is needed in cmdManager
	if (-1 == cmdManager->handleInput())
    {
        theConnection->close();
        unique_lock<mutex> lock(manMapMutexM);
        con2CmdManagerMapM.erase(fd);
        delete cmdManager; 
    }
	return ;
}

//-----------------------------------------------------------------------------

void TelnetProtocol::handleClose(Net::Connection::SocketConnectionPtr theConnection)
{
    LOG_DEBUG("telnet close. fd: " << theConnection->getFd());
    int fd = theConnection->getFd();
    lock_guard<mutex> lock(manMapMutexM);
    Con2CmdManagerMap::iterator it = con2CmdManagerMapM.find(fd);
    if (it != con2CmdManagerMapM.end())
    {
        delete it->second;
        con2CmdManagerMapM.erase(it);
    }

}

//-----------------------------------------------------------------------------

void TelnetProtocol::handleConnected(Connection::SocketConnectionPtr theConnection)
{
    LOG_DEBUG("telnet connected. fd: " << theConnection->getFd());
	TelnetCmdManager* cmdManager = NULL;
    int fd = theConnection->getFd();
    {
        lock_guard<mutex> lock(manMapMutexM);
        Con2CmdManagerMap::iterator it = con2CmdManagerMapM.find(fd);
        if (it == con2CmdManagerMapM.end())
        {
            cmdManager = new TelnetCmdManager(theConnection->getPeerAddr(), theConnection, this);		
            con2CmdManagerMapM[fd] = cmdManager;
        }
        else
        {
            delete it->second;
            cmdManager = new TelnetCmdManager(theConnection->getPeerAddr(), theConnection, this);		
            it->second = cmdManager;
        }
    }
    cmdManager->sendPrompt();

}

//-----------------------------------------------------------------------------

const std::string TelnetProtocol::getAddr()
{
    return ConfigCenter::instance()->get("cmd.s.addr", "127.0.0.1");
}

//-----------------------------------------------------------------------------

int TelnetProtocol::getPort()
{
    return ConfigCenter::instance()->get("cmd.s.port", 7510);
}

//-----------------------------------------------------------------------------

int TelnetProtocol::getRBufferSizePower()
{
    return 12;
}

//-----------------------------------------------------------------------------

int TelnetProtocol::getWBufferSizePower()
{
    return 13;
}

//-----------------------------------------------------------------------------


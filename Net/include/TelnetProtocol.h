#ifndef TELNETPROTOCOL_H
#define TELNETPROTOCOL_H

#include "Protocol.h"
#include "SocketConnection.h"
#include "ICmdHandler.h"
#include "TelnetCmdManager.h"

#include <list>
#include <map>
#include <string>
#include <mutex>

namespace nd
{
	class CppProcessor;
	typedef std::map<int, TelnetCmdManager*> Con2CmdManagerMap;

    class TelnetProtocol : public IProtocol
    {
    public:
		TelnetProtocol(CppProcessor* theProcessor);
        TelnetProtocol();
        ~TelnetProtocol();

        void handleInput(SocketConnectionPtr theConnection);
        void handleClose(SocketConnectionPtr theConnection); 
        void handleConnected(SocketConnectionPtr theConnection);

        virtual const std::string getAddr();
        virtual int getPort();
        virtual int getRBufferSizePower();
        virtual int getWBufferSizePower();
    private:
		Con2CmdManagerMap con2CmdManagerMapM;
        std::mutex manMapMutexM;
    };
}

#endif


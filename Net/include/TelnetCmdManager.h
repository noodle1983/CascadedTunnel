#ifndef TELNETCMDMANAGER_H
#define TELNETCMDMANAGER_H

#include "Protocol.h"
#include "SocketConnection.h"
#include "ICmdHandler.h"
#include "QuitHandler.h"
#include "HelpHandler.h"

#include <list>
#include <map>
#include <string>
#include <mutex>
#include <memory>

namespace nd
{
	class CppProcessor;

	typedef std::list<ICmdHandler*> CmdHandlerStack;
	typedef std::list<void*>        CmdSessionDataStack;
	typedef std::map<std::string, ICmdHandler*> CmdMap;

	class TelnetCmdManager
	{
	public:
		TelnetCmdManager(const struct sockaddr_in& thePeerAddr,
				SocketConnectionPtr theConnection,
				IProtocol* theProtocol);
		~TelnetCmdManager();

		static void registCmd(
			const std::string& theCmdName,
			ICmdHandler* theHandler);
        static void initTopCmd();

		bool validate(const sockaddr_in& thePeerAddr);

		int handleInput();
        int handleCmd(const unsigned theStart, const unsigned theEnd);
        void send(const char* const theStr, unsigned theLen);
        void sendPrompt(); 

		min_heap_item_t* addLocalTimer(
				const struct timeval& theInterval, 
				TimerCallback theCallback,
				void* theArg)
        {
			return protocolM->addLocalTimer(fdM, 
					theInterval, theCallback, theArg);
        }
		void cancelLocalTimer(
                min_heap_item_t*& theEvent)
        {
			return protocolM->cancelLocalTimer(fdM,theEvent);
		}

        void* takeOverInputHandler(ICmdHandler* theHandler);
        void exitCurCmd();
        void printUsage();
	private:
		static bool isTopCmdsMInitedM;
		static CmdMap allTopCmdsM;
        static std::mutex topCmdMutexM;
        static QuitHandler quitHandlerM;
        static HelpHandler helpHandlerM;

		CmdHandlerStack subCmdStackM;
		CmdSessionDataStack subCmdDataStackM;
        char cmdBufferM[256];
        unsigned bufferLenM;

		struct sockaddr_in peerAddrM;
		SocketConnectionWPtr connectionM;
		int fdM;
		IProtocol* protocolM;

	};
}

#endif


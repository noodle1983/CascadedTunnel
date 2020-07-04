#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "Processor.h"
#include <functional>
#include <memory>

namespace nd
{
    class SocketConnection;
    typedef std::shared_ptr<SocketConnection> SocketConnectionPtr;
    class UdpServer;
    typedef std::shared_ptr<UdpServer> UdpServerPtr;
    class IProtocol
    {
    public:
        IProtocol(CppProcessor* theProcessor)
            : processorM(theProcessor)
        {
        }
        virtual ~IProtocol() {};

        /**
         *
         * interface: asynHandleInput
         * Description: the net framework will notify the protocol object the input event,
         *         For the performance, Protocol should handle the input in another thread.
         * the Args:
         *         theFd: which socket the input is from
         *         connection: the socket connection which can be write to
         *
         */
        void asynHandleInput(const int theFd, SocketConnectionPtr theConnection)
        {
            return processorM->process(theFd + 1,
                    NEW_JOB(&IProtocol::handleInput, this, theConnection));
        }
        void asynHandleClose(const int theFd, SocketConnectionPtr theConnection)
        {
            return processorM->process(theFd + 1,
                    NEW_JOB(&IProtocol::handleClose, this, theConnection));
        }
        void asynHandleConnected(const int theFd, SocketConnectionPtr theConnection)
        {
            return processorM->process(theFd + 1,
                    NEW_JOB(&IProtocol::handleConnected, this, theConnection));
        }
        void asynHandleHeartbeat(const int theFd, SocketConnectionPtr theConnection) 
        {
            return processorM->process(theFd + 1,
                    NEW_JOB(&IProtocol::handleHeartbeat, this, theConnection));
        }
        void asynHandleSent(const int theFd, SocketConnectionPtr theConnection) 
        {
            return processorM->process(theFd + 1,
                    NEW_JOB(&IProtocol::handleSent, this, theConnection));
        }
		inline min_heap_item_t* addLocalTimer(
				const int theFd,
				const struct timeval& theInterval, 
				TimerCallback theCallback,
				void* theArg)
        {
			return processorM->addLocalTimer(theFd + 1, 
					theInterval, theCallback, theArg);
        }
		inline void cancelLocalTimer(
                const int theFd, 
                min_heap_item_t*& theEvent)
        {
			return processorM->cancelLocalTimer(theFd + 1,theEvent);
		}
        
        virtual void handleInput(SocketConnectionPtr theConnection) = 0;
        virtual void handleSent(SocketConnectionPtr theConnection){};
        virtual void handleClose(SocketConnectionPtr theConnection){}
        virtual void handleConnected(SocketConnectionPtr theConnection){}
        /*
         * send heartbeat msg in heartbeat 
         * or close the Connection if the connection is no response.
         */
        virtual void handleHeartbeat(SocketConnectionPtr theConnection) {}

        //Config
        virtual const std::string getAddr(){ return "0.0.0.0"; }
        virtual int getPort(){ return 5460; }
        virtual int getRBufferSizePower(){ return 20; }
        virtual int getWBufferSizePower(){ return 20; }
        virtual int getHeartbeatInterval(){ return 0; }
        virtual int getMaxHeartbeatTimeout(){ return 3; }
        
    private:
        CppProcessor* processorM;
    };

    class IClientProtocol: public IProtocol
    {
    public:
        IClientProtocol(CppProcessor* theProcessor)
            :IProtocol(theProcessor)
        {
        }
        virtual ~IClientProtocol() {};

        //Config
        virtual unsigned getReConnectInterval(){ return 5; }
        virtual const std::string getAddr(){ return "127.0.0.1"; }
    };

    class IUdpProtocol
    {
    public:
        IUdpProtocol(CppProcessor* theProcessor)
            : processorM(theProcessor)
        {
        }
        virtual ~IUdpProtocol() {};

        /**
         *
         * interface: asynHandleInput
         * Description: the net framework will notify the protocol object the input event,
         *         For the performance, Protocol should handle the input in another thread.
         * the Args:
         *         theFd: which socket the input is from
         *         connection: the socket connection which can be write to
         *
         */
        void asynHandleInput(int theFd, UdpServerPtr theUdpServer)
        {
            return processorM->process(theFd,
                    NEW_JOB(&IUdpProtocol::handleInput, this, theUdpServer));
        }
        
        virtual void handleInput(UdpServerPtr theUdpServer) = 0;
        
        //Config
        virtual int getRBufferSizePower(){ return 20; }
    private:
        CppProcessor* processorM;
    };
}

#endif /*PROTOCOL_H*/


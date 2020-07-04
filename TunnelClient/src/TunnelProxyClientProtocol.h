#ifndef TUNNELSERVERPROTOCOL_H
#define TUNNELSERVERPROTOCOL_H

#include "Protocol.h"
#include "SocketConnection.h"

#include <set>
#include <map>
#include <string>

namespace nd
{
	class CppProcessor;
    class TunnelClientProtocol;
    class TunnelProxyClientProtocol : public IClientProtocol
    {
    public:
		TunnelProxyClientProtocol(CppProcessor* theProcessor, TunnelClientProtocol* clientProtocol);
        TunnelProxyClientProtocol();
        ~TunnelProxyClientProtocol();

        void handleInput(SocketConnectionPtr theConnection);
        void handleSent(SocketConnectionPtr theConnection);
        void handleClose(SocketConnectionPtr theConnection); 
        void handleConnected(SocketConnectionPtr theConnection);

        virtual const std::string getAddr();
        virtual int getPort();
        virtual int getRBufferSizePower();
        virtual int getWBufferSizePower();
        virtual unsigned getReConnectInterval(){ return 0; }
    private:
        TunnelClientProtocol* clientProtocolM;
    };
}

#endif


#ifndef TUNNELPROXYPROTOCOL_H
#define TUNNELPROXYPROTOCOL_H

#include "Protocol.h"
#include "SocketConnection.h"
#include "TunnelServerProtocol.h"

#include <list>
#include <map>
#include <string>

namespace nd
{
	class CppProcessor;
    class TunnelProxyProtocol : public IProtocol
    {
    public:
		TunnelProxyProtocol(CppProcessor* theProcessor, TunnelServerProtocol* theServerProtocol);
        TunnelProxyProtocol();
        ~TunnelProxyProtocol();

        void handleInput(SocketConnectionPtr theConnection);
        void handleSent(SocketConnectionPtr theConnection);
        void handleClose(SocketConnectionPtr theConnection); 
        void handleConnected(SocketConnectionPtr theConnection);

        virtual const std::string getAddr();
        virtual int getPort();
        virtual int getRBufferSizePower();
        virtual int getWBufferSizePower();
    private:
        TunnelServerProtocol* serverProtocolM;
    };
}

#endif


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

        const std::string getAddr(size_t param) override;
        int getPort(size_t param) override;
        int getRBufferSizePower() override;
        int getWBufferSizePower() override;
        unsigned getReConnectInterval() override { return 0; }
    private:
        TunnelClientProtocol* clientProtocolM;
    };
}

#endif


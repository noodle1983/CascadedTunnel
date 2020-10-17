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

        const std::string getAddr(size_t param) override;
        int getPort(size_t param) override;
        int getRBufferSizePower() override;
        int getWBufferSizePower() override;
    private:
        TunnelServerProtocol* serverProtocolM;
    };
}

#endif


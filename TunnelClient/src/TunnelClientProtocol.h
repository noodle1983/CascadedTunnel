#ifndef TUNNELPROXYPROTOCOL_H
#define TUNNELPROXYPROTOCOL_H

#include "Protocol.h"
#include "SocketConnection.h"
#include "TcpClient.h"
#include "TunnelProxyClientProtocol.h"

#include <list>
#include <map>
#include <string>

namespace nd
{
	class CppProcessor;
	typedef std::map<int, TcpClient*> ProxyToConnectionMap;
	typedef std::map<TcpClient*, int> ConnectionToProxyFdMap;
    class TunnelClientProtocol : public IClientProtocol
    {
    public:
		TunnelClientProtocol(CppProcessor* theProcessor);
        TunnelClientProtocol();
        void release();
        ~TunnelClientProtocol();

        void handleInput(SocketConnectionPtr theConnection);
        void handleClose(SocketConnectionPtr theConnection); 
        void handleConnected(SocketConnectionPtr theConnection);
        void handleHeartbeat(SocketConnectionPtr theConnection);

        void handleProxyInput(SocketConnectionPtr theConnection);
        void handleProxySent(SocketConnectionPtr theConnection);
        void handleProxyClose(SocketConnectionPtr theConnection); 
        void handleProxyConnected(SocketConnectionPtr theConnection);

        const std::string getAddr(size_t param) override;
        int getPort(size_t param) override;
        int getRBufferSizePower() override;
        int getWBufferSizePower() override;
        int getHeartbeatInterval() override { return 5; }
    private:
        TcpClientPtr client2ServerM;
        ProxyToConnectionMap proxyToConnectionM;
        ConnectionToProxyFdMap connectionToProxyM;
        TunnelProxyClientProtocol proxyClientProtocolM;
    };
}

#endif


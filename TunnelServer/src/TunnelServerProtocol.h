#ifndef TUNNELSERVERPROTOCOL_H
#define TUNNELSERVERPROTOCOL_H

#include "Protocol.h"
#include "SocketConnection.h"

#include <set>
#include <map>
#include <string>
#include <vector>

namespace nd
{
	class CppProcessor;
	class TcpServer;
	class TunnelProxyProtocol;
    struct ConnectionPair
    {
        SocketConnectionPtr proxyConnectionM;
        SocketConnectionPtr peerConnectionM;
    };

    class TunnelProxyProtocol;
	typedef std::set<SocketConnectionPtr> ConnectionSet;
	typedef std::map<int, ConnectionPair> ConnectionMap;
	typedef std::vector<TcpServer*> ProxyServerVector;
    class TunnelServerProtocol : public IProtocol
    {
    public:
		TunnelServerProtocol(CppProcessor* theProcessor);
        TunnelServerProtocol();
        ~TunnelServerProtocol();

        void handleInput(SocketConnectionPtr theConnection);
        void handleClose(SocketConnectionPtr theConnection); 
        void handleConnected(SocketConnectionPtr theConnection);
        void handleHeartbeat(SocketConnectionPtr theConnection);

        void startProxyServers();
        void stopProxyServers();

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
        ConnectionSet peerConnectionSetM; 
        ConnectionMap proxyFd2InfoMapM; 

        ProxyServerVector proxyServersM;
        TunnelProxyProtocol* proxyProtocolM;
    };
}

#endif


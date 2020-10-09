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

        virtual const std::string getAddr();
        virtual int getPort();
        virtual int getRBufferSizePower();
        virtual int getWBufferSizePower();
        virtual int getHeartbeatInterval(){ return 5; }
    private:
        TcpClientPtr client2ServerM;
        ProxyToConnectionMap proxyToConnectionM;
        ConnectionToProxyFdMap connectionToProxyM;
        TunnelProxyClientProtocol proxyClientProtocolM;
    };
}

#endif


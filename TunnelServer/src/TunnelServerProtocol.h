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
    struct ConnectionPair
    {
        SocketConnectionPtr proxyConnectionM;
        SocketConnectionPtr peerConnectionM;
    };

    class TunnelProxyProtocol;
	typedef std::set<SocketConnectionPtr> ConnectionSet;
	typedef std::map<int, ConnectionPair> ConnectionMap;
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

        void setProxyProtocol(TunnelProxyProtocol* theProtocol){proxyProtocolM = theProtocol;}
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
        ConnectionSet peerConnectionSetM; 
        ConnectionMap proxyFd2InfoMapM; 
        TunnelProxyProtocol* proxyProtocolM;
    };
}

#endif


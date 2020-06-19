#ifndef TUNNELPROXYPROTOCOL_H
#define TUNNELPROXYPROTOCOL_H

#include "Protocol.h"
#include "SocketConnection.h"
#include "TunnelProxyClientProtocol.h"

#include <list>
#include <map>
#include <string>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

namespace Processor
{
	class BoostProcessor;
}

namespace Net
{
namespace Protocol
{
	typedef std::map<int, TcpClient*> ProxyToConnectionMap;
	typedef std::map<TcpClient*, int> ConnectionToProxyFdMap;
    class TunnelClientProtocol : public Net::IClientProtocol
    {
    public:
		TunnelClientProtocol(Processor::BoostProcessor* theProcessor);
        TunnelClientProtocol();
        ~TunnelClientProtocol();

        void handleInput(Connection::SocketConnectionPtr theConnection);
        void handleClose(Net::Connection::SocketConnectionPtr theConnection); 
        void handleConnected(Connection::SocketConnectionPtr theConnection);

        void handleProxyInput(Connection::SocketConnectionPtr theConnection);
        void handleProxyClose(Net::Connection::SocketConnectionPtr theConnection); 
        void handleProxyConnected(Connection::SocketConnectionPtr theConnection);

        virtual const std::string getAddr();
        virtual int getPort();
        virtual int getRBufferSizePower();
        virtual int getWBufferSizePower();
    private:
        TcpClient client2ServerM;
        ProxyToConnectionMap proxyToConnectionM;
        ConnectionToProxyFdMap connectionToProxyM;
        TunnelProxyClientProtocol proxyClientProtocolM;
    };

}
}

#endif


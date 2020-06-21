#ifndef TUNNELSERVERPROTOCOL_H
#define TUNNELSERVERPROTOCOL_H

#include "Protocol.h"
#include "SocketConnection.h"

#include <set>
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
    struct ConnectionPair
    {
        Connection::SocketConnectionPtr proxyConnectionM;
        Connection::SocketConnectionPtr peerConnectionM;
    };

	typedef std::set<Connection::SocketConnectionPtr> ConnectionSet;
	typedef std::map<int, ConnectionPair> ConnectionMap;
    class TunnelServerProtocol : public Net::IProtocol
    {
    public:
		TunnelServerProtocol(Processor::BoostProcessor* theProcessor);
        TunnelServerProtocol();
        ~TunnelServerProtocol();

        void handleInput(Connection::SocketConnectionPtr theConnection);
        void handleClose(Net::Connection::SocketConnectionPtr theConnection); 
        void handleConnected(Connection::SocketConnectionPtr theConnection);
        void handleHeartbeat(Connection::SocketConnectionPtr theConnection);

        void handleProxyInput(Connection::SocketConnectionPtr theConnection);
        void handleProxyClose(Net::Connection::SocketConnectionPtr theConnection); 
        void handleProxyConnected(Connection::SocketConnectionPtr theConnection);

        virtual const std::string getAddr();
        virtual int getPort();
        virtual int getRBufferSizePower();
        virtual int getWBufferSizePower();
    private:
        ConnectionSet peerConnectionSetM; 
        ConnectionMap proxyFd2InfoMapM; 
    };

}
}

#endif


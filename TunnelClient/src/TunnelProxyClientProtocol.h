#ifndef TUNNELSERVERPROTOCOL_H
#define TUNNELSERVERPROTOCOL_H

#include "Protocol.h"
#include "SocketConnection.h"

#include <set>
#include <map>
#include <string>

namespace Processor
{
	class BoostProcessor;
}

namespace Net
{
namespace Protocol
{
    class TunnelClientProtocol;
    class TunnelProxyClientProtocol : public Net::IClientProtocol
    {
    public:
		TunnelProxyClientProtocol(Processor::BoostProcessor* theProcessor, TunnelClientProtocol* clientProtocol);
        TunnelProxyClientProtocol();
        ~TunnelProxyClientProtocol();

        void handleInput(Connection::SocketConnectionPtr theConnection);
        void handleSent(Connection::SocketConnectionPtr theConnection);
        void handleClose(Net::Connection::SocketConnectionPtr theConnection); 
        void handleConnected(Connection::SocketConnectionPtr theConnection);

        virtual const std::string getAddr();
        virtual int getPort();
        virtual int getRBufferSizePower();
        virtual int getWBufferSizePower();
        virtual unsigned getReConnectInterval(){ return 0; }
    private:
        TunnelClientProtocol* clientProtocolM;
    };

}
}

#endif


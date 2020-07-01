#ifndef TUNNELPROXYPROTOCOL_H
#define TUNNELPROXYPROTOCOL_H

#include "Protocol.h"
#include "SocketConnection.h"
#include "TunnelServerProtocol.h"

#include <list>
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
    class TunnelProxyProtocol : public Net::IProtocol
    {
    public:
		TunnelProxyProtocol(Processor::BoostProcessor* theProcessor, TunnelServerProtocol* theServerProtocol);
        TunnelProxyProtocol();
        ~TunnelProxyProtocol();

        void handleInput(Connection::SocketConnectionPtr theConnection);
        void handleSent(Connection::SocketConnectionPtr theConnection);
        void handleClose(Net::Connection::SocketConnectionPtr theConnection); 
        void handleConnected(Connection::SocketConnectionPtr theConnection);

        virtual const std::string getAddr();
        virtual int getPort();
        virtual int getRBufferSizePower();
        virtual int getWBufferSizePower();
    private:
        TunnelServerProtocol* serverProtocolM;
    };

}
}

#endif


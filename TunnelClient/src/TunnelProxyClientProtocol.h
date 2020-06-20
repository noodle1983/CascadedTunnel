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
    class TunnelClientProtocol;
    class TunnelProxyClientProtocol : public Net::IClientProtocol
    {
    public:
		TunnelProxyClientProtocol(Processor::BoostProcessor* theProcessor, TunnelClientProtocol* clientProtocol);
        TunnelProxyClientProtocol();
        ~TunnelProxyClientProtocol();

        void handleInput(Connection::SocketConnectionPtr theConnection);
        void handleClose(Net::Connection::SocketConnectionPtr theConnection); 
        void handleConnected(Connection::SocketConnectionPtr theConnection);

        virtual const std::string getAddr();
        virtual int getPort();
        virtual int getRBufferSizePower();
        virtual int getWBufferSizePower();
    private:
        TunnelClientProtocol* clientProtocolM;
    };

}
}

#endif


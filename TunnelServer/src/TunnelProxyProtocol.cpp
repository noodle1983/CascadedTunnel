#include "TunnelProxyProtocol.h"
#include "ConfigCenter.h"
#include "SocketConnection.h"
#include "Log.h"

using namespace Net;
using namespace Net::Protocol;
using namespace Config;


//-----------------------------------------------------------------------------

TunnelProxyProtocol::TunnelProxyProtocol(Processor::BoostProcessor* theProcessor, TunnelServerProtocol* theServerProtocol)
	: IProtocol(theProcessor)
    , serverProtocolM(theServerProtocol)
{
}

//-----------------------------------------------------------------------------

TunnelProxyProtocol::~TunnelProxyProtocol()
{
}

//-----------------------------------------------------------------------------

void TunnelProxyProtocol::handleInput(Connection::SocketConnectionPtr theConnection)
{
    serverProtocolM->handleProxyInput(theConnection);
}

//-----------------------------------------------------------------------------

void TunnelProxyProtocol::handleClose(Net::Connection::SocketConnectionPtr theConnection)
{
    serverProtocolM->handleProxyClose(theConnection);
}

//-----------------------------------------------------------------------------

void TunnelProxyProtocol::handleConnected(Connection::SocketConnectionPtr theConnection)
{
    serverProtocolM->handleProxyConnected(theConnection);
}

//-----------------------------------------------------------------------------

const std::string TunnelProxyProtocol::getAddr()
{
    return ConfigCenter::instance()->get("proxy.s.addr", "127.0.0.1");
}

//-----------------------------------------------------------------------------

int TunnelProxyProtocol::getPort()
{
    return ConfigCenter::instance()->get("proxy.s.port", 5460);
}

//-----------------------------------------------------------------------------

int TunnelProxyProtocol::getRBufferSizePower()
{
    return 12;
}

//-----------------------------------------------------------------------------

int TunnelProxyProtocol::getWBufferSizePower()
{
    return 17;
}

//-----------------------------------------------------------------------------


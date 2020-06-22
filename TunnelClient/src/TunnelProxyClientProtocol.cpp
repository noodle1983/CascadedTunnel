#include "TunnelProxyClientProtocol.h"
#include "TunnelClientProtocol.h"
#include "ConfigCenter.h"
#include "SocketConnection.h"
#include "Message.h"
#include "Log.h"

using namespace Net;
using namespace Net::Protocol;
using namespace Config;
using namespace Msg;


//-----------------------------------------------------------------------------

TunnelProxyClientProtocol::TunnelProxyClientProtocol(Processor::BoostProcessor* theProcessor, TunnelClientProtocol* clientProtocol)
	: IClientProtocol(theProcessor)
    , clientProtocolM(clientProtocol)
{
}

//-----------------------------------------------------------------------------

TunnelProxyClientProtocol::~TunnelProxyClientProtocol()
{
}

//-----------------------------------------------------------------------------

void TunnelProxyClientProtocol::handleInput(Connection::SocketConnectionPtr theConnection)
{
    clientProtocolM->handleProxyInput(theConnection);
	return ;
}

//-----------------------------------------------------------------------------

void TunnelProxyClientProtocol::handleClose(Net::Connection::SocketConnectionPtr theConnection)
{
    clientProtocolM->handleProxyClose(theConnection);
}

//-----------------------------------------------------------------------------

void TunnelProxyClientProtocol::handleConnected(Connection::SocketConnectionPtr theConnection)
{
    clientProtocolM->handleProxyConnected(theConnection);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

const std::string TunnelProxyClientProtocol::getAddr()
{
    return ConfigCenter::instance()->get("proxy.c.addr", "127.0.0.1");
}

//-----------------------------------------------------------------------------

int TunnelProxyClientProtocol::getPort()
{
    return ConfigCenter::instance()->get("proxy.c.port", 5466);
}

//-----------------------------------------------------------------------------

int TunnelProxyClientProtocol::getRBufferSizePower()
{
    return 12;
}

//-----------------------------------------------------------------------------

int TunnelProxyClientProtocol::getWBufferSizePower()
{
    return 13;
}

//-----------------------------------------------------------------------------


#include "TunnelProxyClientProtocol.h"
#include "TunnelClientProtocol.h"
#include "ConfigCenter.h"
#include "SocketConnection.h"
#include "Message.h"
#include "Log.h"

using namespace nd;
using namespace Msg;


//-----------------------------------------------------------------------------

TunnelProxyClientProtocol::TunnelProxyClientProtocol(CppProcessor* theProcessor, TunnelClientProtocol* clientProtocol)
	: IClientProtocol(theProcessor)
    , clientProtocolM(clientProtocol)
{
}

//-----------------------------------------------------------------------------

TunnelProxyClientProtocol::~TunnelProxyClientProtocol()
{
}

//-----------------------------------------------------------------------------

void TunnelProxyClientProtocol::handleInput(SocketConnectionPtr theConnection)
{
    clientProtocolM->handleProxyInput(theConnection);
	return ;
}

//-----------------------------------------------------------------------------

void TunnelProxyClientProtocol::handleSent(SocketConnectionPtr theConnection)
{
    clientProtocolM->handleProxySent(theConnection);
	return ;
}


//-----------------------------------------------------------------------------

void TunnelProxyClientProtocol::handleClose(SocketConnectionPtr theConnection)
{
    clientProtocolM->handleProxyClose(theConnection);
}

//-----------------------------------------------------------------------------

void TunnelProxyClientProtocol::handleConnected(SocketConnectionPtr theConnection)
{
    clientProtocolM->handleProxyConnected(theConnection);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

const std::string TunnelProxyClientProtocol::getAddr(size_t param)
{
    return g_cfg->get("proxy.c.addr", "127.0.0.1");
}

//-----------------------------------------------------------------------------

int TunnelProxyClientProtocol::getPort(size_t param)
{
    auto cfgPortVector = g_cfg->getIntVector("proxy.c.port");
    return cfgPortVector[param]; 
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


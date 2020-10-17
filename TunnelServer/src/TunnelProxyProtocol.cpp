#include "TunnelProxyProtocol.h"
#include "ConfigCenter.h"
#include "SocketConnection.h"
#include "Log.h"

using namespace nd;


//-----------------------------------------------------------------------------

TunnelProxyProtocol::TunnelProxyProtocol(CppProcessor* theProcessor, TunnelServerProtocol* theServerProtocol)
	: IProtocol(theProcessor)
    , serverProtocolM(theServerProtocol)
{
}

//-----------------------------------------------------------------------------

TunnelProxyProtocol::~TunnelProxyProtocol()
{
}

//-----------------------------------------------------------------------------

void TunnelProxyProtocol::handleInput(SocketConnectionPtr theConnection)
{
    serverProtocolM->handleProxyInput(theConnection);
}

//-----------------------------------------------------------------------------

void TunnelProxyProtocol::handleSent(SocketConnectionPtr theConnection)
{
    serverProtocolM->handleProxySent(theConnection);
}

//-----------------------------------------------------------------------------

void TunnelProxyProtocol::handleClose(SocketConnectionPtr theConnection)
{
    serverProtocolM->handleProxyClose(theConnection);
}

//-----------------------------------------------------------------------------

void TunnelProxyProtocol::handleConnected(SocketConnectionPtr theConnection)
{
    serverProtocolM->handleProxyConnected(theConnection);
}

//-----------------------------------------------------------------------------

const std::string TunnelProxyProtocol::getAddr(size_t param)
{
    return g_cfg->get("proxy.s.addr", "127.0.0.1");
}

//-----------------------------------------------------------------------------

int TunnelProxyProtocol::getPort(size_t param)
{
    auto cfgPortVector = g_cfg->getIntVector("proxy.s.port");
    return cfgPortVector[param]; 
}

//-----------------------------------------------------------------------------

int TunnelProxyProtocol::getRBufferSizePower()
{
    return 12;
}

//-----------------------------------------------------------------------------

int TunnelProxyProtocol::getWBufferSizePower()
{
    return 13;
}

//-----------------------------------------------------------------------------


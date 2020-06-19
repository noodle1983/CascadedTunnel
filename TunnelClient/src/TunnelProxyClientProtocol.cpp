#include "TunnelProxyClientProtocol.h"
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
	: IProtocol(theProcessor)
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
    LOG_DEBUG("tunnel client close. fd: " << theConnection->getFd());
    clientProtocolM->handleProxyClose(theConnection);
}

//-----------------------------------------------------------------------------

void TunnelProxyClientProtocol::handleConnected(Connection::SocketConnectionPtr theConnection)
{
    LOG_DEBUG("tunnel client connected. fd: " << theConnection->getFd());
    clientProtocolM->handleConnected(theConnection);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

const std::string TunnelProxyClientProtocol::getAddr()
{
    return ConfigCenter::instance()->get("cmd.s.addr", "127.0.0.1");
}

//-----------------------------------------------------------------------------

int TunnelProxyClientProtocol::getPort()
{
    return ConfigCenter::instance()->get("cmd.s.port", 7510);
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


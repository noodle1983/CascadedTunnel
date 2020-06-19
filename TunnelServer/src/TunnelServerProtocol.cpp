#include "TunnelServerProtocol.h"
#include "ConfigCenter.h"
#include "SocketConnection.h"
#include "Message.h"
#include "Log.h"

using namespace Net;
using namespace Net::Protocol;
using namespace Config;
using namespace Msg;


//-----------------------------------------------------------------------------

TunnelServerProtocol::TunnelServerProtocol(Processor::BoostProcessor* theProcessor)
	:IProtocol(theProcessor)
{
}

//-----------------------------------------------------------------------------

TunnelServerProtocol::~TunnelServerProtocol()
{
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::handleInput(Connection::SocketConnectionPtr theConnection)
{
    char buffer[16 * 1024] = {0};
    while(true)
    {
        //not enough input
        if (MsgHeader::MIN_BYTES > theConnection->getRBufferSize()) { return; }

        theConnection->peeknInput(buffer, Msg::MsgHeader::MIN_BYTES);

        MsgHeader header;
        unsigned decodeLength = 0;
        if (header.decode(buffer, 0, decodeLength) != SUCCESS_E){
            LOG_ERROR("failed to decode header.");
            theConnection->close();
            return;
        }

        uint32_t length = header.length;
        if (length > 10 * 1024 || length < Msg::MsgHeader::MIN_BYTES){
            LOG_ERROR("Msg is invalid:" << length << ". the msg's edge may be lost, close connection!");
            theConnection->close();
            return ;
        }

        //not enough input
        if (length > theConnection->getRBufferSize()) { return ; }
        theConnection->getnInput(buffer, length);

        int proxyFd = header.proxyFd;
        ConnectionMap::iterator it = proxyFd2InfoMapM.find(proxyFd);
        if (it == proxyFd2InfoMapM.end()){
            LOG_WARN("no proxy connection found. ignore");
            continue;
        }

        if (ProxyRsp::ID == header.messageType) {
            ProxyRsp msg;
            if (msg.decode(buffer, 0, decodeLength) != SUCCESS_E) {
                LOG_ERROR("decode ProxyRsp error");
                theConnection->close();
                return;
            }

            unsigned len = it->second.proxyConnectionM->sendn(msg.payload.valueM.c_str(), msg.payload.valueM.length()); 
            if (len != msg.payload.valueM.length()) {
                LOG_WARN("speed mismatch. ignore");
                continue;
            }

        }
        else{
            LOG_ERROR("unknow msg type:" << header.messageType);
            theConnection->close();
            return;
        }
    }

	return ;
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::handleClose(Net::Connection::SocketConnectionPtr theConnection)
{
    LOG_DEBUG("tunnel client close. fd: " << theConnection->getFd());
    peerConnectionSetM.erase(theConnection);
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::handleConnected(Connection::SocketConnectionPtr theConnection)
{
    LOG_DEBUG("tunnel client connected. fd: " << theConnection->getFd());
    peerConnectionSetM.insert(theConnection);
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::handleProxyInput(Connection::SocketConnectionPtr theConnection)
{
    LOG_DEBUG("proxy client input. fd: " << theConnection->getFd());
    ConnectionMap::iterator it = proxyFd2InfoMapM.find(theConnection->getFd());
    if (it == proxyFd2InfoMapM.end()){
        theConnection->close();
        LOG_WARN("no peer connection found. close proxy: " << theConnection->getFd());
        return;
    }

    if (it->second.peerConnectionM->isClose()){
        theConnection->close();
        LOG_WARN("peer connection closed. close proxy: " << theConnection->getFd());
        return;
    }

    char buffer[2048] = {0};
    while(true){
        unsigned len = theConnection->getRBufferSize();
        if (len == 0) {return;}
        if (len > 1024){len = 1024;}
        
        ProxyReq msg;
        msg.init();
        msg.proxyFd = theConnection->getFd();
        theConnection->getnInput(buffer, len);
        msg.payload.valueM.assign(buffer, len);
        unsigned encodeIndex = 0;
        msg.encode(buffer, 0, encodeIndex);
        it->second.peerConnectionM->sendn(buffer, encodeIndex);
    }
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::handleProxyClose(Net::Connection::SocketConnectionPtr theConnection)
{
    LOG_DEBUG("proxy client closed. fd: " << theConnection->getFd());
    proxyFd2InfoMapM.erase(theConnection->getFd());
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::handleProxyConnected(Connection::SocketConnectionPtr theConnection)
{
    LOG_DEBUG("proxy client connected. fd: " << theConnection->getFd());
    if (peerConnectionSetM.empty())
    {
        theConnection->close();
        LOG_WARN("no peer client connected. close proxy: " << theConnection->getFd());
        return;
    }
    ConnectionPair conPair;
    conPair.proxyConnectionM = theConnection;
    conPair.peerConnectionM = *peerConnectionSetM.begin();
    proxyFd2InfoMapM[theConnection->getFd()] = conPair;

    NewConnection msg;
    msg.init();
    msg.proxyFd = theConnection->getFd();
    msg.messageType = NewConnection::ID;

    char buffer[NewConnection::MIN_BYTES] = {0};
    unsigned endIndex = 0;
    msg.encode(buffer, NewConnection::MIN_BYTES, endIndex);
    conPair.peerConnectionM->sendn(buffer, endIndex);
}


//-----------------------------------------------------------------------------

const std::string TunnelServerProtocol::getAddr()
{
    return ConfigCenter::instance()->get("inner.s.addr", "127.0.0.1");
}

//-----------------------------------------------------------------------------

int TunnelServerProtocol::getPort()
{
    return ConfigCenter::instance()->get("inner.s.port", 5461);
}

//-----------------------------------------------------------------------------

int TunnelServerProtocol::getRBufferSizePower()
{
    return 12;
}

//-----------------------------------------------------------------------------

int TunnelServerProtocol::getWBufferSizePower()
{
    return 13;
}

//-----------------------------------------------------------------------------


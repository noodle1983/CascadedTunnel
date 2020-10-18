#include "TunnelServerProtocol.h"
#include "ConfigCenter.h"
#include "SocketConnection.h"
#include "Message.h"
#include "Log.h"
#include "TunnelProxyProtocol.h"
#include "TcpServer.h"
#include "Reactor.h"

#include <vector>

using namespace std;
using namespace nd;
using namespace Msg;

//-----------------------------------------------------------------------------

TunnelServerProtocol::TunnelServerProtocol(CppProcessor* theProcessor)
	:IProtocol(theProcessor)
{
    proxyProtocolM = new TunnelProxyProtocol(g_net_processor, this);
}

//-----------------------------------------------------------------------------

TunnelServerProtocol::~TunnelServerProtocol()
{
    delete proxyProtocolM;
    proxyProtocolM = NULL;
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::startProxyServers()
{
    auto cfgPortVector = g_cfg->getIntVector("proxy.s.port");
    for(unsigned i = 0; i < cfgPortVector.size(); i++){
        TcpServer* server = new TcpServer(proxyProtocolM, g_reactor, g_net_processor);
        server->setProtocolParam(i);
        server->start();
        proxyServersM.push_back(server);
    }
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::stopProxyServers()
{
    for(auto server : proxyServersM){
        server->stop();
        delete server;
    }
    proxyServersM.clear();
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::handleInput(SocketConnectionPtr theConnection)
{
    char buffer[16 * 1024] = {0};
    while(true)
    {
        //not enough input
        if (MsgHeader::MIN_BYTES > theConnection->getRBufferSize()) { return; }

        theConnection->peeknInput(buffer, Msg::MsgHeader::MIN_BYTES);

        MsgHeader header;
        unsigned decodeLength = 0;
        if (header.decodeHeader(buffer, Msg::MsgHeader::MIN_BYTES, decodeLength) != SUCCESS_E){
            LOG_ERROR("failed to decode header.");
            theConnection->close();
            return;
        }

        uint32_t length = header.length;
        if (length > 10 * 1024 || length < Msg::MsgHeader::MIN_BYTES){
            LOG_ERROR("Msg: "  << header.messageType.valueM << " is with invalid len:" << length
                    << ". the msg's edge may be lost, close connection!");
            theConnection->close();
            return ;
        }

        //not enough input
        if (length > theConnection->getRBufferSize()) { return ; }
        theConnection->getnInput(buffer, length);

        //inner msg
        theConnection->resetHeartbeatTimeoutCounter();
        if (HeartbeatReq::ID == header.messageType) {
            HeartbeatRsp msg(0);
            theConnection->sendMsg(msg);
            continue;
        }

        //proxy msg
        int proxyFd = header.proxyFd;
        ConnectionMap::iterator it = proxyFd2InfoMapM.find(proxyFd);
        if (it == proxyFd2InfoMapM.end()){
            LOG_WARN("no proxy connection found. ignore");
            continue;
        }
        else if (SyncWinSize::ID == header.messageType) {
            SyncWinSize msg;
            decodeLength = 0;
            if (msg.decode(buffer, length, decodeLength) != SUCCESS_E) {
                LOG_ERROR("decode SyncWinSize error");
                theConnection->close();
                return;
            }

            LOG_TRACE("upflow SyncWinSize, winOffset:" << msg.winOffset.valueM << ", proxyFd:" << proxyFd);
            SocketConnectionPtr con = it->second.proxyConnectionM;
            con->setUpperData((void*)(uintptr_t)msg.winOffset.valueM);
            con->getProtocol()->asynHandleInput(proxyFd, con);
            continue;
        }
        else if (RProxyConClose::ID == header.messageType) {
            LOG_DEBUG("RProxyConClose:" << proxyFd);
            it->second.proxyConnectionM->setUpperData((void*)0);
            it->second.proxyConnectionM->setCloseAfterSent();
            continue;
        }
        else if (ProxyRsp::ID == header.messageType) {
            LOG_TRACE("ProxyRsp, len:" << length << ", proxyFd:" << proxyFd);
            ProxyRsp msg;
            decodeLength = 0;
            if (msg.decode(buffer, length, decodeLength) != SUCCESS_E) {
                LOG_ERROR("decode ProxyRsp error");
                theConnection->close();
                return;
            }

            SocketConnectionPtr proxyConnection = it->second.proxyConnectionM;
            unsigned len = proxyConnection->sendn(msg.payload.valueM.c_str(), msg.payload.valueM.length()); 
            if (len != msg.payload.valueM.length()) {
                LOG_ERROR("speed mismatch!!! please enlarge buffer");
                theConnection->close();
                continue;
            }

        }
        else{
            LOG_ERROR("unknow msg type:" << header.messageType.valueM);
            theConnection->close();
            return;
        }
    }

	return ;
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::handleClose(SocketConnectionPtr theConnection)
{
    LOG_DEBUG("tunnel client close. fd: " << theConnection->getFd());
    peerConnectionSetM.erase(theConnection);

    vector<int> invalidFds;
    ConnectionMap::iterator it = proxyFd2InfoMapM.begin();
    for(; it != proxyFd2InfoMapM.end(); it++)
    {
        if (it->second.peerConnectionM.get() == theConnection.get())
        {
            invalidFds.push_back(it->first);
            it->second.proxyConnectionM->setUpperData((void*)0);
            it->second.proxyConnectionM->close();
        }
    }

    for(unsigned i = 0; i < invalidFds.size(); i++)
    {
        proxyFd2InfoMapM.erase(invalidFds[i]);
    }
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::handleConnected(SocketConnectionPtr theConnection)
{
    LOG_DEBUG("tunnel client connected. fd: " << theConnection->getFd());
    peerConnectionSetM.insert(theConnection);
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::handleHeartbeat(SocketConnectionPtr theConnection)
{
    int hbcounter = theConnection->getHeartbeatTimeoutCounter();
    if (hbcounter > 3){
        theConnection->close();
        LOG_DEBUG("connection to Tunnel server reach max heartbeat:" << hbcounter
                << ", fd: " << theConnection->getFd());
        return;
    }

    if (hbcounter > 1)
    {
        LOG_DEBUG("heartbeat dida. fd: " << theConnection->getFd() << ", counter:" << hbcounter);
    }
    theConnection->incHeartbeatTimeoutCounter();
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::handleProxyInput(SocketConnectionPtr theConnection)
{
    int proxyFd = theConnection->getFd();
    LOG_TRACE("proxy client input. fd: " << proxyFd);
    ConnectionMap::iterator it = proxyFd2InfoMapM.find(proxyFd);
    if (it == proxyFd2InfoMapM.end()){
        theConnection->close();
        LOG_WARN("no peer connection found. close proxy: " << proxyFd);
        return;
    }

    SocketConnectionPtr peerConnection = it->second.peerConnectionM;
    if (peerConnection->isClose()){
        theConnection->close();
        LOG_WARN("peer connection closed. close proxy: " << proxyFd);
        return;
    }

    int32_t winSize = (uint32_t)(uintptr_t)(theConnection->getUpperData()) - (uint32_t)theConnection->getReadedBytes();
	bool canWrite = peerConnection->isWBufferHealthy();
    char buffer[2048] = {0};
    while(canWrite && winSize > 0){
        int32_t len = (int32_t)theConnection->getRBufferSize();
        if (len == 0) {break;}
        if (len > winSize){len = winSize;}
        if (len > 1024){len = 1024;}
        winSize -= len;
        
        ProxyReq msg(0);
        msg.proxyFd = proxyFd;
        theConnection->getnInput(buffer, len);
        msg.payload.valueM.assign(buffer, len);
        peerConnection->sendMsg(msg);
        canWrite = peerConnection->isWBufferHealthy();
        LOG_TRACE("ProxyReq len:" << msg.length.valueM << ", winSize:" << winSize << ". fd: " << proxyFd);
    }
    LOG_TRACE("upflow SyncWinSize after sent, winSize:" << winSize << ", proxyFd:" << proxyFd);

    if (!canWrite && !peerConnection->hasWatcher(proxyFd))
    {
        peerConnection->setLowWaterMarkWatcher(proxyFd, new Watcher(bind(
            &TunnelProxyProtocol::asynHandleInput, theConnection->getProtocol(), proxyFd, theConnection)));
    }
}


//-----------------------------------------------------------------------------

void TunnelServerProtocol::handleProxySent(SocketConnectionPtr theConnection)
{
    ConnectionMap::iterator it = proxyFd2InfoMapM.find(theConnection->getFd());
    if (it == proxyFd2InfoMapM.end()) {
        return;
    }

    SyncWinSize msg(0);
    msg.proxyFd = theConnection->getFd();
    msg.winOffset = theConnection->getWBufferSpace() + theConnection->getWritenBytes();
    it->second.peerConnectionM->sendMsg(msg);
    LOG_TRACE("update downflow SyncWinSize, winOffset:" << msg.winOffset.valueM << ", proxyFd:" << theConnection->getFd());
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::handleProxyClose(SocketConnectionPtr theConnection)
{
    LOG_DEBUG("proxy client closed. fd: " << theConnection->getFd());
    theConnection->setUpperData((void*)0);
    ConnectionMap::iterator it = proxyFd2InfoMapM.find(theConnection->getFd());
    if (it == proxyFd2InfoMapM.end()) {
        return;
    }
    ConnectionPair& conPair = it->second;

    ConnectionClosed msg(0);
    msg.proxyFd = theConnection->getFd();
    conPair.peerConnectionM->sendMsg(msg);

    proxyFd2InfoMapM.erase(it);
}

//-----------------------------------------------------------------------------

void TunnelServerProtocol::handleProxyConnected(SocketConnectionPtr theConnection)
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

    NewConnection msg(0);
    msg.proxyFd = theConnection->getFd();
    msg.winOffset = theConnection->getWBufferSpace();
    msg.protoParam = (uint32_t)theConnection->getProtocolParam();
    conPair.peerConnectionM->sendMsg(msg);
}


//-----------------------------------------------------------------------------

const std::string TunnelServerProtocol::getAddr(size_t param)
{
    return g_cfg->get("inner.s.addr", "127.0.0.1");
}

//-----------------------------------------------------------------------------

int TunnelServerProtocol::getPort(size_t param)
{
    return g_cfg->get("inner.s.port", 5461);
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


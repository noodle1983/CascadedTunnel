#include "TunnelClientProtocol.h"
#include "ConfigCenter.h"
#include "SocketConnection.h"
#include "Log.h"
#include "BoostProcessor.h"
#include "TcpClient.h"
#include "Reactor.h"
#include "Message.h"

using namespace std;
using namespace Net;
using namespace Net::Protocol;
using namespace Config;
using namespace Msg;
using namespace Processor;


//-----------------------------------------------------------------------------

TunnelClientProtocol::TunnelClientProtocol(Processor::BoostProcessor* theProcessor)
	: IClientProtocol(theProcessor)
    , client2ServerM(this, g_reactor, theProcessor)
    , proxyClientProtocolM(theProcessor, this)
{
    client2ServerM.connect();
}

//-----------------------------------------------------------------------------

TunnelClientProtocol::~TunnelClientProtocol()
{
}

//-----------------------------------------------------------------------------

void TunnelClientProtocol::handleInput(Connection::SocketConnectionPtr theConnection)
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
        ProxyToConnectionMap::iterator it = proxyToConnectionM.find(proxyFd);
        if (it == proxyToConnectionM.end()){
            LOG_WARN("no proxy connection found. ignore");
            continue;
        }

        if (NewConnection::ID == header.messageType) {
            NewConnection msg;
            if (msg.decode(buffer, 0, decodeLength) != SUCCESS_E) {
                LOG_ERROR("decode ProxyRsp error");
                theConnection->close();
                return;
            }

            TcpClient* client = new TcpClient(&proxyClientProtocolM, g_reactor, BoostProcessor::netInstance()); 
            client->connect();    

            ProxyToConnectionMap::iterator it = proxyToConnectionM.find(msg.proxyFd);
            if (it != proxyToConnectionM.end()){
                LOG_WARN("dup client connection found. del");
                connectionToProxyM.erase(it->second);
                delete it->second;
            }

            proxyToConnectionM[msg.proxyFd] = client;
            connectionToProxyM[client] = msg.proxyFd; 
        }
        else if (ProxyReq::ID == header.messageType) {
            ProxyReq msg;
            if (msg.decode(buffer, 0, decodeLength) != SUCCESS_E) {
                LOG_ERROR("decode ProxyRsp error");
                theConnection->close();
                return;
            }

            ProxyToConnectionMap::iterator it = proxyToConnectionM.find(msg.proxyFd);
            if (it == proxyToConnectionM.end()){
                LOG_WARN("dup client connection found. del");
                continue;
            }

            TcpClient* client = it->second;
            string& payload = msg.payload.valueM;
            unsigned sent = client->sendn(payload.c_str(), payload.length());
            if(sent != payload.length())
            {
                LOG_WARN("data dropped! fd:" << msg.proxyFd << ", sent:" << sent);
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

void TunnelClientProtocol::handleClose(Net::Connection::SocketConnectionPtr theConnection)
{
    LOG_DEBUG("disconnected from Tunnel server. fd: " << theConnection->getFd());
    ProxyToConnectionMap::iterator it = proxyToConnectionM.begin();
    for(; it != proxyToConnectionM.end(); it++)
    {
        TcpClient* client = it->second;
        delete client;
    }
    proxyToConnectionM.clear();
    connectionToProxyM.clear();
}

//-----------------------------------------------------------------------------

void TunnelClientProtocol::handleConnected(Connection::SocketConnectionPtr theConnection)
{
    LOG_DEBUG("connected to Tunnel server. fd: " << theConnection->getFd());
}

//-----------------------------------------------------------------------------

void TunnelClientProtocol::handleProxyInput(Connection::SocketConnectionPtr theConnection)
{
    TcpClient* client = theConnection->getClient();
    ConnectionToProxyFdMap::iterator it = connectionToProxyM.find(client);
    if (it == connectionToProxyM.end())
    {
        client->close();
        LOG_DEBUG("handle proxy input. client to server lost! fd:" << theConnection->getFd());
        return;
    }

    if (!client2ServerM.isConnected()){
        client->close();
        LOG_WARN("peer connection closed. close proxy: " << theConnection->getFd());
        return;
    }

    char buffer[2048] = {0};
    while(true){
        unsigned len = theConnection->getRBufferSize();
        if (len == 0) {return;}
        if (len > 1024){len = 1024;}
        
        ProxyRsp msg;
        msg.init();
        msg.proxyFd = it->second;
        theConnection->getnInput(buffer, len);
        msg.payload.valueM.assign(buffer, len);
        unsigned encodeIndex = 0;
        msg.encode(buffer, 0, encodeIndex);
        client2ServerM.sendn(buffer, encodeIndex);
    }
}

//-----------------------------------------------------------------------------

void TunnelClientProtocol::handleProxyClose(Net::Connection::SocketConnectionPtr theConnection)
{
    TcpClient* client = theConnection->getClient();
    ConnectionToProxyFdMap::iterator it = connectionToProxyM.find(client);
    if (it == connectionToProxyM.end())
    {
        LOG_DEBUG("client deleted. fd:" << theConnection->getFd());
        return;
    }
    int fd = it->second;
    proxyToConnectionM.erase(fd);
    connectionToProxyM.erase(it);
}

//-----------------------------------------------------------------------------

void TunnelClientProtocol::handleProxyConnected(Connection::SocketConnectionPtr theConnection)
{
    TcpClient* client = theConnection->getClient();
    ConnectionToProxyFdMap::iterator it = connectionToProxyM.find(client);
    if (it == connectionToProxyM.end())
    {
        LOG_ERROR("client should be deleted. fd:" << theConnection->getFd());
        return;
    }
    int fd = it->second;
    
}

//-----------------------------------------------------------------------------

const std::string TunnelClientProtocol::getAddr()
{
    return ConfigCenter::instance()->get("cmd.c.addr", "127.0.0.1");
}

//-----------------------------------------------------------------------------

int TunnelClientProtocol::getPort()
{
    return ConfigCenter::instance()->get("cmd.c.port", 5461);
}

//-----------------------------------------------------------------------------

int TunnelClientProtocol::getRBufferSizePower()
{
    return 12;
}

//-----------------------------------------------------------------------------

int TunnelClientProtocol::getWBufferSizePower()
{
    return 13;
}

//-----------------------------------------------------------------------------

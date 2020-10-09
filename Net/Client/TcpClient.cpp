#include "TcpClient.h"
#include "SocketConnection.h"
#include "Protocol.h"
#include "Log.h"
#include "Reactor.h"
#include "ConfigCenter.h"

#include <event.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

using namespace nd;
using namespace std;

#ifndef WIN32
/* True iff e is an error that means an connect can be retried. */
#define ERR_CONNECT_RETRIABLE(e)         \
        ((e) == EINTR || (e) == EINPROGRESS)

#else

#define ERR_CONNECT_RETRIABLE(e)                 \
        ((e) == WSAEWOULDBLOCK ||                   \
                (e) == WSAEINTR ||                      \
                (e) == WSAEINPROGRESS ||                    \
                (e) == WSAEINVAL)

#endif

//-----------------------------------------------------------------------------

TcpClient::TcpClient(
            IClientProtocol* theProtocol,
            Reactor* theReactor,
            CppProcessor* theProcessor)
    : protocolM(theProtocol)
    , reactorM(theReactor)
    , processorM(theProcessor)
    , peerAddrM(theProtocol->getAddr())
    , peerPortM(theProtocol->getPort())
    , isClosedM(0)
    , isConnectedM(0)
    , selfM(this)
    , reconnectTimerEvtM(NULL)
    , connectTimesM(0)
{
    LOG_DEBUG("new client: 0x" << std::hex << this << ". " << peerAddrM << ":" << std::dec << peerPortM);
    processorIdM = (uint64_t)this >> 3;
}

//-----------------------------------------------------------------------------

TcpClient::~TcpClient()
{
    LOG_DEBUG("release client: 0x" << std::hex << this << ". " << peerAddrM << ":" << std::dec << peerPortM);
    close();
}

//-----------------------------------------------------------------------------

void TcpClient::deleteSelf()
{
    close();
    processorM->PROCESS(processorIdM, &TcpClient::_deleteSelf, this); 
}

//-----------------------------------------------------------------------------

void TcpClient::_deleteSelf()
{
    if (reconnectTimerEvtM){processorM->cancelLocalTimer(processorIdM, reconnectTimerEvtM);}
    reconnectTimerEvtM = NULL;
    selfM.reset();
}

//-----------------------------------------------------------------------------

int TcpClient::close()
{
    isClosedM = true;
    isConnectedM = false;

    processorM->PROCESS(processorIdM, &TcpClient::_close, this); 
    return 0;
}

//-----------------------------------------------------------------------------

void TcpClient::_close()
{
    if (connectionM.get())
    {
        connectionM->rmClient();
        connectionM->close();
        connectionM.reset();
    }
}

//-----------------------------------------------------------------------------

int TcpClient::connect(void* theUpperData)
{
    if (isClosedM)
    {
        LOG_ERROR("TcpClient to[" << peerAddrM << ":" << peerPortM << "is already closed");
        return -1;
    }
    //init attr
    isConnectedM = false;
    processorM->PROCESS(processorIdM, &TcpClient::_connect, this, theUpperData); 
    return 0;
}

//-----------------------------------------------------------------------------

void TcpClient::_connect(void* theUpperData)
{
    if (isClosedM) { return; }
    LOG_DEBUG("connecting to " << peerAddrM << ":" << peerPortM);

    _close();

    //init attr
    isConnectedM = false;

    evutil_socket_t sock;
    struct sockaddr_in sin;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    evutil_inet_pton(AF_INET, peerAddrM.c_str(), &sin.sin_addr);
    sin.sin_port = htons(peerPortM);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        LOG_ERROR("failed to do socket(AF_INET,...)!"
                    << ", errstr:" << evutil_socket_error_to_string(errno));
        return;
    }
    if (evutil_make_socket_nonblocking(sock) < 0)
    {
        LOG_ERROR("failed to make socket monblocking!"
                    << ", errstr:" << evutil_socket_error_to_string(errno));
        evutil_closesocket(sock);
        return;
    }
    connectTimesM++;
    if (::connect(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0)
    {
        int e = errno;
        if (! ERR_CONNECT_RETRIABLE(e))
        {
            LOG_WARN("failed to connect to " << peerAddrM
                    << ":" << peerPortM
                    << ", errstr:" << evutil_socket_error_to_string(e));
            evutil_closesocket(sock);
            onError(connectionM);
            return;
        }
    }
    else
    {
        //connected
        isConnectedM = true;
    }
    SocketConnection* connection =
        new SocketConnection(protocolM, reactorM, processorM, sock, this);
    connection->setUpperData(theUpperData);
    connectionM = connection->self();
    if (!isConnectedM)
    {
        connectionM->addClientTimer(protocolM->getReConnectInterval());
    }
}


//-----------------------------------------------------------------------------

void TcpClient::onConnected(int theFd, SocketConnectionPtr theConnection)
{
    if (isClosedM) { return ; }

    isConnectedM = true;
    protocolM->asynHandleConnected(theFd, theConnection);

    LOG_DEBUG("connected to " << peerAddrM << ":" << peerPortM);
}

//-----------------------------------------------------------------------------
static void reconnect(void* client)
{
    TcpClient* self = (TcpClient*)client;
    self->resetTimer();
    self->connect();
}

void TcpClient::_reconnectLater()
{
    if (isClosedM) { return; }

    //reconnect
    _close();
    isConnectedM = false;

    struct timeval tv;
    tv.tv_sec = protocolM->getReConnectInterval(); 
    tv.tv_usec = 0;
    if (reconnectTimerEvtM){processorM->cancelLocalTimer(processorIdM, reconnectTimerEvtM);}
    reconnectTimerEvtM = processorM->addLocalTimer(processorIdM, tv, reconnect, this); 
}

//-----------------------------------------------------------------------------

void TcpClient::onError(SocketConnectionPtr theConnection)
{
    if (connectionM.get() != theConnection.get()) { return; }
    LOG_WARN("connection lost from " << peerAddrM << ":" << peerPortM);

    if (isClosedM) { return; }

    if (protocolM->getReConnectInterval() > 0)
    {
        processorM->PROCESS(processorIdM, &TcpClient::_reconnectLater, this); 
    }

}

//-----------------------------------------------------------------------------

void TcpClient::onClientTimeout()
{
    if (isClosedM) { return; }
    if (!isConnectedM)
    {
        processorM->PROCESS(processorIdM, &reconnect, (void*)this); 
    }
}

//-----------------------------------------------------------------------------


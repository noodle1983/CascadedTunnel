#include "Processor.h"
#include "TcpServer.h"
#include "SocketConnection.h"
#include "Reactor.h"
#include "Log.h"
#include "Protocol.h"
#include "App.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

using namespace nd;

//-----------------------------------------------------------------------------
//libevent
//-----------------------------------------------------------------------------
void on_accept(int theFd, short theEvt, void *theArg)
{
    TcpServer* server = (TcpServer*)theArg;
    server->asynAccept(theFd, theEvt);
}

//-----------------------------------------------------------------------------
//TcpServer
//-----------------------------------------------------------------------------

TcpServer::TcpServer(
        IProtocol* theProtocol,
        Reactor* theReactor,
        CppProcessor* theProcessor)
    : protocolM(theProtocol)
    , reactorM(theReactor)
    , processorM(theProcessor)
    , acceptEvtM(NULL)
    , portM(0)
    , fdM(0)
    , protocolParamM(-1)
{
    LOG_DEBUG("new server: " << std::hex << this << "." << std::dec);
}

//-----------------------------------------------------------------------------


TcpServer::~TcpServer()
{
    LOG_DEBUG("release server: " << std::hex << this << ". listen port:" << std::dec << portM);
    stop();
}

//-----------------------------------------------------------------------------

void TcpServer::addAcceptEvent()
{
    if (-1 == event_add(acceptEvtM, NULL))
    {
        processorM->PROCESS(fdM, &TcpServer::addAcceptEvent, this);
    }
}

//-----------------------------------------------------------------------------

void TcpServer::asynAccept(int theFd, short theEvt)
{
    processorM->PROCESS(fdM, &TcpServer::onAccept, this, theFd, theEvt);
} 

//-----------------------------------------------------------------------------


void TcpServer::onAccept(int theFd, short theEvt)
{
    evutil_socket_t clientFd;
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    clientFd = accept(theFd, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientFd < 0)
    {
        LOG_WARN("accept failed");
        return;
    }
    while(clientFd >= 0)
    {
        if (evutil_make_socket_nonblocking(clientFd) < 0)
        {
            LOG_WARN("failed to set client socket non-blocking");
            return;
        }
        SocketConnection* connection = new SocketConnection(protocolM, reactorM, processorM, clientFd);
		connection->setPeerAddr(&clientAddr);
        connection->setProtocolParam(protocolParamM);
        char addrBuffer[16] = {0};
        LOG_DEBUG("Accepted connection on " << portM
                << " from "<< inet_ntop(AF_INET, &clientAddr.sin_addr, addrBuffer, sizeof(addrBuffer)) << ":" << clientAddr.sin_port
                << ", fd:" << clientFd
                << ", con addr:" << std::hex << (size_t)connection
                << std::dec);

        clientFd = accept(theFd, (struct sockaddr *)&clientAddr, &clientLen);
    }
    addAcceptEvent();

}

//-----------------------------------------------------------------------------


int TcpServer::start()
{
    //new a socket
    fdM = socket(AF_INET, SOCK_STREAM, 0);
    if (fdM < 0)
    {
        LOG_FATAL("listen failed on " << portM);
        exit(-1);
    }
    //set socket option
    if (evutil_make_listen_socket_reuseable(fdM) < 0)
    {
        LOG_FATAL("failed to set server socket to reuseable");
        exit(-1);
    }
    if (evutil_make_socket_nonblocking(fdM) < 0)
    {
        LOG_FATAL("failed to set server socket to non-blocking");
        exit(-1);
    }
    int nRecvBuf=32*1024;
    setsockopt (fdM, SOL_SOCKET, SO_RCVBUF,(const char*)&nRecvBuf, sizeof(int));

    //bind local addr
    struct sockaddr_in listenAddr;
    memset(&listenAddr, 0, sizeof(listenAddr));
    listenAddr.sin_family = AF_INET;
    evutil_inet_pton(AF_INET, protocolM->getAddr(protocolParamM).c_str(), &listenAddr.sin_addr);
    portM = protocolM->getPort(protocolParamM);
    listenAddr.sin_port = htons(portM);
    //listenAddr.sin_addr.s_addr = INADDR_ANY;
    //listenAddr.sin_port = htons(portM);
    if (bind(fdM, (struct sockaddr *)&listenAddr,
        sizeof(listenAddr)) < 0)
    {
        LOG_FATAL("bind failed on " << protocolM->getAddr(protocolParamM) 
                << ":" << protocolM->getPort(protocolParamM)
                << ", errno:" << errno);
        g_app->fini();
        exit(-1);
    }

    //listen
    if (listen(fdM, 5) < 0)
    {
        LOG_FATAL("listen failed");
        exit(-1);
    }

    acceptEvtM = reactorM->newEvent(fdM, EV_READ, on_accept, this);
    addAcceptEvent();

    LOG_DEBUG("Server has been listening at port " << portM);
    return 0;
}

//-----------------------------------------------------------------------------

void TcpServer::stop()
{
    if (fdM)
    {
        evutil_closesocket(fdM);
        reactorM->delEvent(acceptEvtM);
        fdM = 0;
    }
}


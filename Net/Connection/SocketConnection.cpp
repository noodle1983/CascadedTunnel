#include "Processor.h"
#include "SocketConnection.h"
#include "TcpClient.h"
#include "Reactor.h"
#include "Protocol.h"
#include "Log.h"

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <err.h>
#include <assert.h>


using namespace nd;
using namespace std;

//-----------------------------------------------------------------------------

void on_read(int theFd, short theEvt, void *theArg)
{
    SocketConnection* connection = (SocketConnection*)theArg;
    connection->asynRead(theFd, theEvt);
}

//-----------------------------------------------------------------------------

void on_write(int theFd, short theEvt, void *theArg)
{
    SocketConnection* connection = (SocketConnection*)theArg;
    connection->asynWrite(theFd, theEvt);
}

//-----------------------------------------------------------------------------

SocketConnection::SocketConnection(
            IProtocol* theProtocol,
            Reactor* theReactor,
            CppProcessor* theProcessor,
            evutil_socket_t theFd)
    : selfM(this)
    , heartbeatTimerEvtM(NULL)
    , clientTimerEvtM(NULL)
    , heartbeatTimeoutCounterM(0)
    , protocolM(theProtocol)
    , reactorM(theReactor)
    , processorM(theProcessor)
    , fdM(theFd)
    , inputQueueM(theProtocol->getRBufferSizePower())
    , outputQueueM(theProtocol->getWBufferSizePower())
    , statusM(ActiveE)
    , stopReadingM(false)
    , clientM(NULL)
    , isConnectedNotified(true)
    , writenBytesM(0)
    , readedBytesM(0)
    , uppperDataM(NULL)
{
    readEvtM = reactorM->newEvent(fdM, EV_READ, on_read, this);
    writeEvtM = reactorM->newEvent(fdM, EV_WRITE, on_write, this);
    addReadEvent();
    protocolM->asynHandleConnected(fdM, selfM);
    processorM->PROCESS(fdM, &SocketConnection::startHeartbeatTimer, this);
    LOG_DEBUG("new connection: " << std::hex << this << ". fd:" << std::dec << fdM);
}

//-----------------------------------------------------------------------------

SocketConnection::SocketConnection(
            IProtocol* theProtocol,
            Reactor* theReactor,
            CppProcessor* theProcessor,
            evutil_socket_t theFd,
            TcpClient* theClient)
    : selfM(this)
    , heartbeatTimerEvtM(NULL)
    , clientTimerEvtM(NULL)
    , heartbeatTimeoutCounterM(0)
    , protocolM(theProtocol)
    , reactorM(theReactor)
    , processorM(theProcessor)
    , fdM(theFd)
    , inputQueueM(theProtocol->getRBufferSizePower())
    , outputQueueM(theProtocol->getWBufferSizePower())
    , statusM(ActiveE)
    , stopReadingM(false)
    , clientM(theClient->self())
    , isConnectedNotified(false)
    , writenBytesM(0)
    , readedBytesM(0)
    , uppperDataM(NULL)
{
    readEvtM = reactorM->newEvent(fdM, EV_READ, on_read, this);
    writeEvtM = reactorM->newEvent(fdM, EV_WRITE, on_write, this);
    addWriteEvent();
    addReadEvent();
    LOG_DEBUG("new connection: " << std::hex << this << ". fd:" << std::dec << fdM);
}
//-----------------------------------------------------------------------------

SocketConnection::~SocketConnection()
{
    evutil_closesocket(fdM);
    clearAllWatchers();
    if (uppperDataM != NULL)
    {
        LOG_WARN("uppperDataM is not NULL and may leek, please free it and set to NULL in upper handling while connection is closed.");
    }
    LOG_DEBUG("release connection: " << std::hex << this << ". fd:" << std::dec << fdM);
}

//-----------------------------------------------------------------------------

void SocketConnection::rmClient()
{
    clientM.reset();
}

//-----------------------------------------------------------------------------

void SocketConnection::addReadEvent()
{
    if (CloseE == statusM)
        return;
    if (-1 == event_add(readEvtM, NULL))
    {
        processorM->PROCESS(fdM, &SocketConnection::addReadEvent, this);
    }
}

//-----------------------------------------------------------------------------

void SocketConnection::addWriteEvent()
{
    if (CloseE == statusM)
        return;
    if (-1 == event_add(writeEvtM, NULL))
    {
        processorM->PROCESS(fdM, &SocketConnection::addWriteEvent, this);
    }
}

//-----------------------------------------------------------------------------

int SocketConnection::asynRead(int theFd, short theEvt)
{
    processorM->PROCESS(fdM, &SocketConnection::onRead, this, theFd, theEvt);
    return 0;
}

//-----------------------------------------------------------------------------

void SocketConnection::onRead(int theFd, short theEvt)
{
    char buffer[1024]= {0};
    unsigned readBufferLeft = inputQueueM.unusedSize();
    unsigned readLen = (readBufferLeft < sizeof(buffer)) ? readBufferLeft : sizeof(buffer);
    if (readLen == 0)
    {
        if (!stopReadingM)
        {
            lock_guard<mutex> lock(stopReadingMutexM);
            stopReadingM = true;
        }
        protocolM->asynHandleInput(fdM, selfM);
        return;
    }

    int len = read(theFd, buffer, readLen);
    if ((0 == len) ||(len < 0 && errno != EWOULDBLOCK))
    {
        LOG_DEBUG("Client disconnected. fd:" << fdM);
        _close();
        return;
    }
    else if (len < 0 && errno == EWOULDBLOCK)
    {
        len = 0;
    }
    unsigned putLen = inputQueueM.put(buffer, len);
    assert(putLen == (unsigned)len);

    while(len > 0 && (BufferOkE == inputQueueM.getStatus()
                    || BufferLowE == inputQueueM.getStatus() ))
    {
        readBufferLeft = inputQueueM.unusedSize();
        readLen = (readBufferLeft < sizeof(buffer)) ? readBufferLeft : sizeof(buffer);
        len = read(theFd, buffer, readLen);
        if (len <= 0)
        {
            break;
        }
        putLen = inputQueueM.put(buffer, len);
        assert(putLen == (unsigned)len);
    }

    if (BufferHighE == inputQueueM.getStatus()
            || BufferNotEnoughE == inputQueueM.getStatus())
    {
        //TRACE("Flow Control:Socket " << fdM << " stop reading.", fdM);
        lock_guard<mutex> lock(stopReadingMutexM);
        stopReadingM = true;
    }
    else
    {
        addReadEvent();
    }
    protocolM->asynHandleInput(fdM, selfM);
}

//-----------------------------------------------------------------------------

unsigned SocketConnection::getInput(char* const theBuffer, const unsigned theLen)
{
    //if (CloseE == statusM)
    //    return 0;

    unsigned len = inputQueueM.get(theBuffer, theLen);
    if (stopReadingM && CloseE != statusM)
    {
        BufferStatus postBufferStatus = inputQueueM.getStatus();
        if (postBufferStatus == BufferLowE)
        {
            {
                lock_guard<mutex> lock(stopReadingMutexM);
                stopReadingM = false;
            }
            asynRead(fdM, 0);
            //processorM->PROCESS(fdM, &SocketConnection::addReadEvent, selfM);
        }
    }
    readedBytesM += len;
    return len;
}

//-----------------------------------------------------------------------------

unsigned SocketConnection::getnInput(char* const theBuffer, const unsigned theLen)
{
    //if (CloseE == statusM)
    //    return 0;

    unsigned len = inputQueueM.getn(theBuffer, theLen);
    if (stopReadingM && CloseE != statusM)
    {
        BufferStatus postBufferStatus = inputQueueM.getStatus();
        if (postBufferStatus == BufferLowE)
        {
            {
                lock_guard<mutex> lock(stopReadingMutexM);
                stopReadingM = false;
            }
            asynRead(fdM, 0);
            //processorM->PROCESS(fdM, &SocketConnection::addReadEvent, selfM);
        }
    }
    readedBytesM += len;
    return len;
}

//-----------------------------------------------------------------------------

unsigned SocketConnection::peeknInput(char* const theBuffer, const unsigned theLen)
{
    //if (CloseE == statusM)
    //    return 0;

    return inputQueueM.peekn(theBuffer, theLen);
}

//-----------------------------------------------------------------------------

unsigned SocketConnection::sendn(const char* const theBuffer, const unsigned theLen)
{
    if (CloseE == statusM)
        return 0;

    if (0 == theLen || NULL == theBuffer)
        return 0;

    unsigned len = 0;
    {
        lock_guard<mutex> lock(outputQueueMutexM);
        len = outputQueueM.putn(theBuffer, theLen);
    }
    if (0 == len)
    {
        LOG_WARN("outage of the connection's write queue!");
    }
    processorM->PROCESS(fdM, &SocketConnection::addWriteEvent, selfM);
    writenBytesM += len;
    return len;
}

//-----------------------------------------------------------------------------

int SocketConnection::asynWrite(int theFd, short theEvt)
{
    if (CloseE == statusM)
        return -1;
    processorM->PROCESS(fdM, &SocketConnection::onWrite, this, theFd, theEvt);
    return 0;
}

//-----------------------------------------------------------------------------

void SocketConnection::setLowWaterMarkWatcher(int theFd, Watcher* theWatcher)
{
	//if it is already writable
    BufferStatus bufferStatus = outputQueueM.getStatus();
	if (bufferStatus == BufferLowE)
	{
		(*theWatcher)();
		return;
	}

	//or set theWatcher and add the write event
	{
		lock_guard<mutex> lock(watcherMutexM);
        WatcherMap::iterator it = watcherMapM.find(theFd);
        if (it != watcherMapM.end()){
            delete it->second;
            watcherMapM.erase(it);
        }
		watcherMapM[theFd] = theWatcher;
	}
    if (CloseE != statusM)
    {
        addWriteEvent();
    }
}

//-----------------------------------------------------------------------------

void SocketConnection::rmLowWaterMarkWatcher(int theFd)
{
    lock_guard<mutex> lock(watcherMutexM);
    WatcherMap::iterator it = watcherMapM.find(theFd);
    if (it == watcherMapM.end()){return;}
    delete it->second;
    watcherMapM.erase(it);
}

//-----------------------------------------------------------------------------

void SocketConnection::clearAllWatchers()
{
    lock_guard<mutex> lock(watcherMutexM);
    WatcherMap::iterator it = watcherMapM.begin();
    for(; it != watcherMapM.end(); it++){
        delete it->second;
    }
    watcherMapM.clear();
}

//-----------------------------------------------------------------------------

bool SocketConnection::hasWatcher(int theFd)
{
    lock_guard<mutex> lock(watcherMutexM);
    return watcherMapM.find(theFd) != watcherMapM.end();
}

//-----------------------------------------------------------------------------

void SocketConnection::onWrite(int theFd, short theEvt)
{
    auto client = clientM;
    if (!isConnectedNotified && client.get() && CloseE != statusM)
    {
        client->onConnected(theFd, selfM);
        isConnectedNotified = true;
        startHeartbeatTimer();
    }
    char buffer[1024]= {0};
    unsigned peekLen = outputQueueM.peek(buffer, sizeof(buffer));
    int writeLen = 0;
    while (CloseE != statusM && peekLen > 0)
    {
        writeLen = write(theFd, buffer, peekLen);
        if (writeLen < 0)
        {
            if (errno == EINTR || errno == EAGAIN)
            {
                break;
            }
            else
            {
                LOG_DEBUG("Socket write failure.");
                _close();
                return;
            }
        }
        outputQueueM.commitRead(writeLen);
        peekLen = outputQueueM.peek(buffer, sizeof(buffer));
    }
    protocolM->asynHandleSent(fdM, selfM);

    BufferStatus bufferStatus = outputQueueM.getStatus();
    if (bufferStatus == BufferLowE)
    {
        lock_guard<mutex> lock(watcherMutexM);
        WatcherMap::iterator it = watcherMapM.find(theFd);
        if (it != watcherMapM.end()){
            (*it->second)();
            delete it->second;
            watcherMapM.erase(it);
        }
    }

    if (CloseE != statusM && !outputQueueM.empty())
    {
        addWriteEvent();
    }

}

//-----------------------------------------------------------------------------

void SocketConnection::startHeartbeatTimer()
{
    struct timeval tv;
    tv.tv_sec = protocolM->getHeartbeatInterval(); 
    if (tv.tv_sec <= 0)
        tv.tv_sec = 60;
    tv.tv_usec = 0;

    heartbeatTimerEvtM = processorM->addLocalTimer(fdM, tv, SocketConnection::onHeartbeat, this);
}

//-----------------------------------------------------------------------------

void SocketConnection::onHeartbeat(void *theArg)
{
    SocketConnection* connection = (SocketConnection*) theArg;
    connection->heartbeatTimerEvtM = NULL;

    int heartbeatInterval = connection->protocolM->getHeartbeatInterval();
    if (heartbeatInterval > 0)
    {
        connection->protocolM->asynHandleHeartbeat(connection->fdM, connection->selfM);
    }

    connection->startHeartbeatTimer();
}

//-----------------------------------------------------------------------------

void SocketConnection::addClientTimer(unsigned theSec)
{
    if(theSec == 0) { return; }
    processorM->PROCESS(fdM, &SocketConnection::_addClientTimer, this, theSec);
}

//-----------------------------------------------------------------------------

void SocketConnection::_addClientTimer(unsigned theSec)
{
    if (NULL == clientM.get() || 0 == theSec)
    {
        LOG_DEBUG("client is null or timeout = 0, ignore, fd:" << fdM);
        return;
    }
    if (clientTimerEvtM)
    {
        processorM->cancelLocalTimer(fdM, clientTimerEvtM);
    }

    struct timeval tv;
    tv.tv_sec = theSec; 
    tv.tv_usec = 0;

    clientTimerEvtM = processorM->addLocalTimer(fdM, tv, &SocketConnection::onClientTimeout, this);

}

//-----------------------------------------------------------------------------

void SocketConnection::onClientTimeout(void *theArg)
{
    SocketConnection* connection = (SocketConnection*) theArg;
    connection->clientTimerEvtM = NULL;
    auto client = connection->clientM;
    if (client.get())
    {
        client->onClientTimeout();
    }

}

//-----------------------------------------------------------------------------

void SocketConnection::close()
{
    if (CloseE == statusM)
        return;
    processorM->PROCESS(fdM, &SocketConnection::_close, this);
}

//-----------------------------------------------------------------------------

void SocketConnection::_release()
{
    selfM.reset();
}

//-----------------------------------------------------------------------------

void SocketConnection::_close()
{
    if (CloseE == statusM)
        return;
    statusM = CloseE;

    clearAllWatchers();
    if (heartbeatTimerEvtM)
    {
        processorM->cancelLocalTimer(fdM, heartbeatTimerEvtM);
    }
    if (clientTimerEvtM)
    {
        processorM->cancelLocalTimer(fdM, clientTimerEvtM);
    }
	protocolM->asynHandleClose(fdM, selfM);
    auto client = clientM;
    if (client.get())
    {
        client->onError(selfM);
    }
    reactorM->delEvent(readEvtM);
    reactorM->delEvent(writeEvtM);
    processorM->PROCESS(fdM, &SocketConnection::_release, this);
}

//-----------------------------------------------------------------------------



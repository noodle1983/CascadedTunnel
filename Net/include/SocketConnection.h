#ifndef SOCKETCONNECTION_H
#define SOCKETCONNECTION_H

#include "KfifoBuffer.h"
#include "min_heap.h"

#include <mutex>
#include <functional>
#include <memory>
#include <map>
#include <event.h>

struct timeval;

namespace nd{

    class CppProcessor;
    class IProtocol;
    class Reactor;
    class TcpClient;
    class SocketConnection;

    typedef std::shared_ptr<SocketConnection> SocketConnectionPtr;
    typedef std::shared_ptr<TcpClient> TcpClientPtr;
    typedef std::weak_ptr<SocketConnection> SocketConnectionWPtr;
    typedef std::function<void ()> Watcher;
    typedef std::map<int, Watcher*> WatcherMap;

    class SocketConnection
    {
    public:
        SocketConnection(
            IProtocol* theProtocol,
            Reactor* theReactor,
            CppProcessor* theProcessor,
            evutil_socket_t theFd,
            uint64_t theSessionId = -1);
        ~SocketConnection();

        /**
         * if there is a client, SocketConnection will notify it when connected and error.
         */
        SocketConnection(
            IProtocol* theProtocol,
            Reactor* theReactor,
            CppProcessor* theProcessor,
            evutil_socket_t theFd,
            TcpClient* theClient);

        //interface for reactor
        int asynRead(int theFd, short theEvt);
        int asynWrite(int theFd, short theEvt);

        //interface for upper protocol
        void addClientTimer(unsigned theSec);
        static void onClientTimeout(void *theArg);

        void rmClient();
        SocketConnectionPtr self(){return selfM;}
        void close();
        void setCloseAfterSent(){
            if (getWBufferSize() == 0){close();};
            closeAfterSentM = true;
        }
        inline bool isClose() {return statusM == CloseE;}
        inline bool isRBufferHealthy(){return inputQueueM.isHealthy();};
        inline bool isWBufferHealthy(){return outputQueueM.isHealthy();};
        unsigned getRBufferSize(){return inputQueueM.size();};
        unsigned getWBufferSize(){return outputQueueM.size();};
        unsigned getWBufferSpace(){return outputQueueM.unusedSize();};
        uint64_t getReadedBytes(){return readedBytesM;}
        uint64_t getWritenBytes(){return writenBytesM;}

        unsigned getInput(char* const theBuffer, const unsigned theLen);
        unsigned getnInput(char* const theBuffer, const unsigned theLen);
        unsigned peeknInput(char* const theBuffer, const unsigned theLen);
        unsigned sendn(const char* const theBuffer, const unsigned theLen);

        void setLowWaterMarkWatcher(int theFd, Watcher* theWatcher);
        void rmLowWaterMarkWatcher(int theFd);
        bool hasWatcher(int theFd);

		//attribute
		int getFd(){return fdM;}
        TcpClient* getClient(){return clientM.get();}
		void setPeerAddr(const struct sockaddr_in* theAddr){peerAddrM = *theAddr;}
		const struct sockaddr_in& getPeerAddr(){return peerAddrM;}
        IProtocol* getProtocol(){return protocolM;}

        //heartbeatTimeoutCounterM
        void resetHeartbeatTimeoutCounter(){heartbeatTimeoutCounterM = 0;}
        int incHeartbeatTimeoutCounter(){return ++heartbeatTimeoutCounterM;}
        int getHeartbeatTimeoutCounter(){return heartbeatTimeoutCounterM;}

        void* getUpperData(){return uppperDataM;}
        void setUpperData(void* theUpperData){uppperDataM = theUpperData;}
        size_t getProtocolParam(){return protocolParamM;}
        void setProtocolParam(size_t protocolParam){protocolParamM = protocolParam;}

        template<typename Msg>
        unsigned sendMsg(Msg& msg){
            unsigned encodeIndex = 0;
            char buffer[4096] = {0};
            msg.encode(buffer, sizeof(buffer), encodeIndex);
            if (getWBufferSpace() < encodeIndex){return 0;}
            return sendn(buffer, encodeIndex);
        }

        uint64_t getSessionId() {return sessionIdM;}
    private:
        friend class std::function<void ()>;
        void addReadEvent();
        void addWriteEvent();
        void onRead(int theFd, short theEvt);
        void onWrite(int theFd, short theEvt);

        void _addClientTimer(unsigned theSec);

        void startHeartbeatTimer();
        static void onHeartbeat(void *theArg);

        void _close();
        void _release();

        void clearAllWatchers();

    private:
        //for reactor:
        //  this is enough
        //  we ensure after delete there is no msg from the libevent
        //for uppper protocol:
        //  selfM should be applied
        //  in case there is a msg after connection is delete
        SocketConnectionPtr selfM;

        struct event* readEvtM;
        struct event* writeEvtM;
        min_heap_item_t* heartbeatTimerEvtM;
        min_heap_item_t* clientTimerEvtM;
        int heartbeatTimeoutCounterM;

        IProtocol* protocolM;
        Reactor* reactorM;
        CppProcessor* processorM;

        evutil_socket_t fdM;

        //we ensure there is only 1 thread read/write the input queue
        //std::mutex inputQueueMutexM;
        KfifoBuffer inputQueueM;
        std::mutex outputQueueMutexM;
        KfifoBuffer outputQueueM;

        enum Status{ActiveE = 0, CloseE = 1};
        volatile size_t statusM;
        volatile size_t closeAfterSentM;

        std::mutex stopReadingMutexM;
        bool stopReadingM;
        std::mutex watcherMutexM;
        WatcherMap watcherMapM;

        TcpClientPtr clientM;
        bool isConnectedNotified;

		struct sockaddr_in peerAddrM;
		struct sockaddr_in localAddrM;
        
        //counter
        uint64_t writenBytesM;
        uint64_t readedBytesM;

        //upper data
        void* uppperDataM;
        size_t protocolParamM;
        uint64_t sessionIdM;
    };
}

#endif /*SOCKETCONNECTION_H*/


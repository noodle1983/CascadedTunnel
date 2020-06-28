#ifndef SOCKETCONNECTION_H
#define SOCKETCONNECTION_H

#include "KfifoBuffer.h"
#include "min_heap.h"

#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <event.h>

struct timeval;
namespace Processor
{
    typedef boost::function<void ()> Job;
    class BoostProcessor;
}

namespace Net{

class IProtocol;
namespace Reactor
{
    class Reactor;
}
namespace Client
{
    class TcpClient;
}

namespace Connection
{

    class SocketConnection;
    typedef boost::shared_ptr<SocketConnection> SocketConnectionPtr;
    typedef boost::weak_ptr<SocketConnection> SocketConnectionWPtr;
    typedef boost::function<void ()> Watcher;
    typedef std::map<int, Watcher*> WatcherMap;

    class SocketConnection
    {
    public:
        SocketConnection(
            IProtocol* theProtocol,
            Reactor::Reactor* theReactor,
            Processor::BoostProcessor* theProcessor,
            evutil_socket_t theFd);
        ~SocketConnection();

        /**
         * if there is a client, SocketConnection will notify it when connected and error.
         */
        SocketConnection(
            IProtocol* theProtocol,
            Reactor::Reactor* theReactor,
            Processor::BoostProcessor* theProcessor,
            evutil_socket_t theFd,
            Client::TcpClient* theClient);

        //interface for reactor
        int asynRead(int theFd, short theEvt);
        int asynWrite(int theFd, short theEvt);

        //interface for upper protocol
        void addClientTimer(unsigned theSec);
        static void onClientTimeout(void *theArg);

        void rmClient();
        SocketConnectionPtr self(){return selfM;}
        void close();
        inline bool isClose() {return statusM == CloseE;}
        inline bool isRBufferHealthy(){return inputQueueM.isHealthy();};
        inline bool isWBufferHealthy(){return outputQueueM.isHealthy();};
        unsigned getRBufferSize(){return inputQueueM.size();};
        unsigned getWBufferSize(){return outputQueueM.size();};
        unsigned getWBufferSpace(){return outputQueueM.unusedSize();};

        unsigned getInput(char* const theBuffer, const unsigned theLen);
        unsigned getnInput(char* const theBuffer, const unsigned theLen);
        unsigned peeknInput(char* const theBuffer, const unsigned theLen);
        unsigned sendn(const char* const theBuffer, const unsigned theLen);

        void setLowWaterMarkWatcher(int theFd, Watcher* theWatcher);
        void rmLowWaterMarkWatcher(int theFd);
        bool hasWatcher(int theFd);

		//attribute
		int getFd(){return fdM;}
        Client::TcpClient* getClient(){return clientM;}
		void setPeerAddr(const struct sockaddr_in* theAddr){peerAddrM = *theAddr;}
		const struct sockaddr_in& getPeerAddr(){return peerAddrM;}
        IProtocol* getProtocol(){return protocolM;}

        //heartbeatTimeoutCounterM
        void resetHeartbeatTimeoutCounter(){heartbeatTimeoutCounterM = 0;}
        int incHeartbeatTimeoutCounter(){return ++heartbeatTimeoutCounterM;}
        int getHeartbeatTimeoutCounter(){return heartbeatTimeoutCounterM;}

        void* getUpperData(){return uppperDataM;}
        void setUpperData(void* theUpperData){uppperDataM = theUpperData;}

        template<typename Msg>
        unsigned sendMsg(Msg& msg){
            unsigned encodeIndex = 0;
            char buffer[4096] = {0};
            msg.encode(buffer, sizeof(buffer), encodeIndex);
            if (getWBufferSpace() < encodeIndex){return 0;}
            return sendn(buffer, encodeIndex);
        }
    private:
        friend class boost::function<void ()>;
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
        Reactor::Reactor* reactorM;
        Processor::BoostProcessor* processorM;

        evutil_socket_t fdM;

        //we ensure there is only 1 thread read/write the input queue
        //boost::mutex inputQueueMutexM;
        Utility::KfifoBuffer inputQueueM;
        boost::mutex outputQueueMutexM;
        Utility::KfifoBuffer outputQueueM;

        enum Status{ActiveE = 0, CloseE = 1};
        mutable int statusM;

        boost::mutex stopReadingMutexM;
        bool stopReadingM;
        boost::mutex watcherMutexM;
        WatcherMap watcherMapM;

        Client::TcpClient* clientM;
        boost::mutex clientMutexM;
        bool isConnectedNotified;

		struct sockaddr_in peerAddrM;
		struct sockaddr_in localAddrM;
        
        //upper data
        void* uppperDataM;
    };

}
}

#endif /*SOCKETCONNECTION_H*/


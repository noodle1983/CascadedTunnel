#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "KfifoBuffer.h"
#include "UdpPacket.h"

#include <mutex>
#include <functional>
#include <memory>
#include <event.h>

struct timeval;

namespace nd{

    class CppProcessor;
    class IUdpProtocol;
    class Reactor;

    class UdpServer;
    typedef std::shared_ptr<UdpServer> UdpServerPtr;

    class UdpServer
    {
    public:
        UdpServer(
            IUdpProtocol* theProtocol,
            Reactor* theReactor,
            CppProcessor* theProcessor);
        ~UdpServer();


        //interface for reactor
        int asynRead(int theFd, short theEvt);
        int asynWrite(int theFd, short theEvt);

        //interface for upper protocol
        int startAt(const int thePort);
        void close();
        inline bool isClose() {return statusM == CloseE;}
        bool getAPackage(UdpPacket* thePackage);
        bool sendAPackage(UdpPacket* thePackage);



    private:
        friend class std::function<void ()>;
        void addReadEvent();
        void onRead(int theFd, short theEvt);
        void _close();
        void _release();


    private:
        UdpServerPtr selfM;

        struct event* readEvtM;

        IUdpProtocol* protocolM;
        Reactor* reactorM;
        CppProcessor* processorM;

        evutil_socket_t fdM;

        //we ensure there is only 1 thread read/write the input queue
        //boost::mutex inputQueueMutexM;
        KfifoBuffer inputQueueM;
        std::mutex outputQueueMutexM;

        enum Status{ActiveE = 0, CloseE = 1};
        mutable int statusM;

        std::mutex stopReadingMutexM;
        bool stopReadingM;

    };
}

#endif /*UDPSERVER_H*/


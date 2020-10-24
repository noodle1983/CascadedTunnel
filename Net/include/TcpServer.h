#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <event.h>



namespace nd
{
    class CppProcessor;
    class IProtocol;
    class Reactor;
    class TcpServer
    {
    public:
        TcpServer(
            IProtocol* theProtocol,
            Reactor* theReactor,
            CppProcessor* theProcessor,
            size_t protocolParam = -1);
        virtual ~TcpServer();

        void addAcceptEvent();
        int start();
        void stop();

        void asynAccept(int theFd, short theEvt);
        void onAccept(int theFd, short theEvt);

        uint64_t getSessionId() {return sessionIdM;}

    private:
        IProtocol* protocolM;
        Reactor* reactorM;
        CppProcessor* processorM;

        struct event* acceptEvtM;
        int portM;
        evutil_socket_t fdM;

        size_t protocolParamM;
        uint64_t sessionIdM;
    };
}

#endif /*TCPSERVER_H*/


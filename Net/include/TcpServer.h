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
            CppProcessor* theProcessor);
        virtual ~TcpServer();

        void addAcceptEvent();
        int start();
        void stop();

        void asynAccept(int theFd, short theEvt);
        void onAccept(int theFd, short theEvt);

        void setProtocolParam(size_t param){protocolParamM = param;}

    private:
        IProtocol* protocolM;
        Reactor* reactorM;
        CppProcessor* processorM;

        struct event* acceptEvtM;
        int portM;
        evutil_socket_t fdM;

        size_t protocolParamM;
    };
}

#endif /*TCPSERVER_H*/


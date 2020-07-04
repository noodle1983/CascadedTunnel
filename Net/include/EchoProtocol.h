#ifndef ECHOPROTOCOL_H
#define ECHOPROTOCOL_H

#include "Protocol.h"

namespace nd
{
    class Reactor;
    class CppProcessor;
    class EchoProtocol:public IProtocol
    {
    public:
        EchoProtocol(CppProcessor* theProcessor);
        ~EchoProtocol();

        void handleInput(SocketConnectionPtr connection);

        virtual const std::string getAddr();
        virtual int getPort();
        virtual int getRBufferSizePower();
        virtual int getWBufferSizePower();
        virtual int getHeartbeatInterval();
        virtual void handleHeartbeat(SocketConnectionPtr theConnection); 
    private:
    };
}

#endif /* ECHOPROTOCOL_H */


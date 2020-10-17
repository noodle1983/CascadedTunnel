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

        const std::string getAddr(size_t param = 0) override;
        int getPort(size_t param = 0) override;
        int getRBufferSizePower() override;
        int getWBufferSizePower() override;
        int getHeartbeatInterval() override;
        void handleHeartbeat(SocketConnectionPtr theConnection) override; 
    private:
    };
}

#endif /* ECHOPROTOCOL_H */


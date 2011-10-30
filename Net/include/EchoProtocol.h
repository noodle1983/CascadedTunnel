#ifndef ECHOPROTOCOL_H
#define ECHOPROTOCOL_H

#include "Protocol.h"


namespace Processor
{
    class BoostProcessor;
}
namespace Net
{
    namespace Reactor
    {
        class Reactor;
    }
namespace Protocol
{

    class EchoProtocol:public Net::IProtocol
    {
    public:
        EchoProtocol(
            Reactor::Reactor* theReactor,
            Processor::BoostProcessor* theProcessor);
        ~EchoProtocol();

        int handleInput(Connection::SocketConnectionPtr connection);
    private:
        Reactor::Reactor* reactorM;
        Processor::BoostProcessor* processorM;
    };

}
}

#endif /* ECHOPROTOCOL_H */

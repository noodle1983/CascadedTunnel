#include <Processor.h>
#include <TcpClient.h>
#include <Reactor.h>
#include <Protocol.h>
#include <Log.h>
#include "ConfigCenter.h"
#include "App.h"

using namespace std;
using namespace nd;

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <assert.h>


class SingleDataProtocol: public IClientProtocol
{
public:
    SingleDataProtocol()
        : IClientProtocol(&netProcessorM)
        , proProcessorM(1)
        , netProcessorM(1)
    {
        tcpClientM = new TcpClient(this, &reactorM, &netProcessorM);
        proProcessorM.start();
        netProcessorM.start();
        reactorM.start();
    }

    ~SingleDataProtocol()
    {
    }

    void startTest()
    {
        tcpClientM->connect();
    }

    void stop()
    {
        tcpClientM->deleteSelf();
        proProcessorM.waitStop();
        netProcessorM.waitStop();
        reactorM.stop();
    }

    void handleConnected(SocketConnectionPtr theConnection)
    {
        tcpClientM->sendn("Hello", 5);
    }

    void handleInput(SocketConnectionPtr theConnection)
    {
        char buffer[1024];
        unsigned len = 1;
        len = theConnection->getInput(buffer, sizeof(buffer));
        assert(len == strlen("Hello"));
        assert(0 == memcmp("Hello", buffer, strlen("Hello")));
        tcpClientM->close();
        g_app->manualStop();
    }

    const std::string getAddr()
    {
        return g_cfg->get("echo.c.addr", "127.0.0.1");
    }

    int getPort()
    {
        return g_cfg->get("echo.c.port", 5460);
    }

    int getRBufferSizePower()
    {
        return g_cfg->get("echo.c.rpower", 20);
    }

    int getWBufferSizePower()
    {
        return g_cfg->get("echo.c.wpower", 20);
    }

private:
    Reactor reactorM;
    CppProcessor proProcessorM;
    CppProcessor netProcessorM;
    TcpClient* tcpClientM;
};

int main(int argc, char *argv[])
{
    g_app->parseAndInit(argc, argv);

    SingleDataProtocol singleDataProtocol;
    singleDataProtocol.startTest();

    g_app->wait();

    singleDataProtocol.stop();
    g_app->sleep(10);
    g_app->fini();
    return 0;
}


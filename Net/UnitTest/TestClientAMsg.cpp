#include <BoostProcessor.h>
#include <TcpClient.h>
#include <Reactor.h>
#include <Protocol.h>
#include <Log.h>
#include "ConfigCenter.h"

using namespace Config;
using namespace std;

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <assert.h>

static int closed = false;
static mutex closedMutexM;
static condition_variable closedCondM;
void sig_stop(int sig)
{
    LOG_DEBUG("receive signal " << sig << ". stopping...");
    lock_guard<mutex> lock(closedMutexM);
    closed = true;
    closedCondM.notify_one();
}

class SingleDataProtocol: public Net::IClientProtocol
{
public:
    SingleDataProtocol()
        : IClientProtocol(&netProcessorM)
        , proProcessorM(1)
        , netProcessorM(1)
        , tcpClientM(this, &reactorM, &netProcessorM)
    {
        proProcessorM.start();
        netProcessorM.start();
        reactorM.start();
    }

    ~SingleDataProtocol()
    {
        proProcessorM.stop();
        netProcessorM.stop();
        reactorM.stop();
    }

    void startTest()
    {
        tcpClientM.connect();
        tcpClientM.sendn("Hello", 5);
    }

    void handleInput(Net::Connection::SocketConnectionPtr theConnection)
    {
        char buffer[1024];
        unsigned len = 1;
        len = theConnection->getInput(buffer, sizeof(buffer));
        assert(len == strlen("Hello"));
        assert(0 == memcmp("Hello", buffer, strlen("Hello")));
        tcpClientM.close();
        sig_stop(0);
    }

    const std::string getAddr()
    {
        return ConfigCenter::instance()->get("echo.c.addr", "127.0.0.1");
    }

    int getPort()
    {
        return ConfigCenter::instance()->get("echo.c.port", 5460);
    }

    int getRBufferSizePower()
    {
        return ConfigCenter::instance()->get("echo.c.rpower", 20);
    }

    int getWBufferSizePower()
    {
        return ConfigCenter::instance()->get("echo.c.wpower", 20);
    }

private:
    Net::Reactor::Reactor reactorM;
    Processor::BoostProcessor proProcessorM;
    Processor::BoostProcessor netProcessorM;
    Net::Client::TcpClient tcpClientM;
};

int main()
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    signal(SIGTERM, sig_stop);
    signal(SIGINT, sig_stop);
    evthread_use_pthreads();

    SingleDataProtocol singleDataProtocol;
    singleDataProtocol.startTest();

    unique_lock<mutex> lock(closedMutexM);
    while(!closed)
    {
        closedCondM.wait(lock);
    }

    LOG_DEBUG("TestClient stopped.");
    return 0;
}


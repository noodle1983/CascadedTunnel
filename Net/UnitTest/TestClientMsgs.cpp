#include "Processor.h"
#include "TcpClient.h"
#include "Reactor.h"
#include "Protocol.h"
#include "Log.h"
#include "ConfigCenter.h"
#include "App.h"

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <string.h>
#include <functional>
#include <assert.h>

using namespace std;
using namespace nd;
using namespace std::placeholders;

const uint64_t TEST_TIMES = 1024 * 1024 * 1/2;
class BatchDataProtocol: public IClientProtocol
{
public:
    BatchDataProtocol()
        : IClientProtocol(&netProcessorM)
        , proProcessorM(1)
        , netProcessorM(1)
        , wBufferCountM(0)
        , readIndexM(0)
    {
        tcpClientM = new TcpClient(this, &reactorM, &netProcessorM);
        proProcessorM.start();
        netProcessorM.start();
        reactorM.start();
        for (int i = 0; i < 10; i++)
        {
            memset(bufferM[i], '0' + i, 1024);
        }
    }

    ~BatchDataProtocol()
    {
    }

    void stop()
    {
        tcpClientM->deleteSelf();
        proProcessorM.waitStop();
        netProcessorM.waitStop();
        reactorM.stop();
    }

    void startTest()
    {
        tcpClientM->connect();
    }

    void handleConnected(SocketConnectionPtr theConnection)
    {
        struct timeval tv;
        evutil_gettimeofday(&tv, NULL);
        starttimeM = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        asynSend(theConnection->getFd(), theConnection);
    }

    int asynSend(int theFd, SocketConnectionPtr theConnection)
    {
        proProcessorM.PROCESS(theFd + 1,
                &BatchDataProtocol::send, this, theFd, theConnection);
        return 0;
    }

    void send(int theFd, SocketConnectionPtr theConnection)
    {
		bool canWrite = true;
        while ((canWrite = theConnection->isWBufferHealthy()))
        {
            char* buffer = bufferM[wBufferCountM%10];
            unsigned len = theConnection->sendn(buffer, 1024);
            if (len > 0)
            {
                wBufferCountM++;
                //std::cout << "write buffer count:" << wBufferCountM << std::endl;
                if (wBufferCountM == TEST_TIMES)
                {
                    return;
                }
            }
        }
        if (!canWrite)
        {
            theConnection->setLowWaterMarkWatcher(
                    theFd, new Watcher(bind(&BatchDataProtocol::asynSend, this, theFd, theConnection)));
        }
        return;
    }

    void handleInput(SocketConnectionPtr theConnection)
    {
        char buffer[1024];
        while(1)
        {
            unsigned len = theConnection->getInput(buffer, sizeof(buffer));
            if (0 == len)
            {
                break;
            }
            //std::cout << "read len:" << len << std::endl;
            for (uint64_t i = 0; i < len; i++)
            {
                uint64_t offset = readIndexM + i;
                assert(unsigned(buffer[i]) == ('0' + offset/1024%10));

            }
            readIndexM += len;
            if (readIndexM == (1024 * TEST_TIMES))
            {
                struct timeval tv;
                evutil_gettimeofday(&tv, NULL);
                long long endtime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
                long long interval = endtime - starttimeM;
                std::cout << "send " << TEST_TIMES << " request in "
                    << interval << " msec. tps:" << (TEST_TIMES * 1000 / interval) << std::endl;
                std::cout << "exit" << std::endl;
                tcpClientM->close();
                g_app->manualStop();
            }
        }
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
    uint64_t wBufferCountM;
    uint64_t readIndexM;
    char bufferM[10][1024];
    long long starttimeM;
};

int main(int argc, char *argv[])
{
    g_app->parseAndInit(argc, argv);

    BatchDataProtocol dataProtocol;
    dataProtocol.startTest();

    g_app->wait();

    dataProtocol.stop();
    g_app->sleep(10);
    g_app->fini();

    return 0;
}


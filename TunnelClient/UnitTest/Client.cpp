#include "Processor.h"
#include "TcpServer.h"
#include "Reactor.h"
#include "TunnelClientProtocol.h"
#include "TunnelProxyClientProtocol.h"
#include "ProcessorSensor.h"
#include "Log.h"
#include "ConfigCenter.h"
#include "App.h"

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>

using namespace std;
using namespace nd;

int main(int argc, char *argv[])
{
    g_app->parseAndInit(argc, argv);

    TunnelClientProtocol clientProtocol(g_net_processor);

    g_app->wait();

    clientProtocol.release();
    g_reactor->stop();
    LOG_DEBUG("TunnelClient stopping...");

    g_app->sleep(100);
    LOG_DEBUG("TunnelClient stopped.");
    return 0;
}


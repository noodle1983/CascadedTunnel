#include "Processor.h"
#include "TcpServer.h"
#include "Reactor.h"
#include "TunnelServerProtocol.h"
#include "TunnelProxyProtocol.h"
#include "ProcessorSensor.h"
#include "Log.h"
#include "ConfigCenter.h"
#include "App.h"

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <csignal>

using namespace std;
using namespace nd;

int main(int argc, char *argv[])
{
    g_app->parseAndInit(argc, argv);

    TunnelServerProtocol serverProtocol(g_net_processor);
    TunnelProxyProtocol proxyProtocol(g_net_processor, &serverProtocol);
    serverProtocol.setProxyProtocol(&proxyProtocol);
    TcpServer innerServer(&serverProtocol, g_reactor, g_net_processor);
    TcpServer proxyServer(&proxyProtocol, g_reactor, g_net_processor);
    innerServer.start();
    proxyServer.start();

    g_app->wait();
    g_reactor->stop();
    proxyServer.stop();
    innerServer.stop();

    LOG_DEBUG("TunnelServer stopped.");
    return 0;
}


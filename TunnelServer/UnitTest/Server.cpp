#include "BoostProcessor.h"
#include "TcpServer.h"
#include "Reactor.h"
#include "TunnelServerProtocol.h"
#include "TunnelProxyProtocol.h"
#include "ProcessorSensor.h"
#include "Log.h"
#include "ConfigCenter.h"

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>

using namespace std;
using namespace Processor;
using namespace Net::Reactor;

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

int main()
{
    bool runInBackground = g_cfg->get("process.background", 1);
    if (runInBackground)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            printf("failed to fork!\n");
            return -1;
        }
        else if (pid > 0)
        {
            printf("server start with pid %d.\n", pid);
            return 0;
        }
        setsid();
    }

    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    signal(SIGTERM, sig_stop);
    signal(SIGINT, sig_stop);
    evthread_use_pthreads();
    Net::Protocol::TunnelServerProtocol serverProtocol(BoostProcessor::netInstance());
    Net::Protocol::TunnelProxyProtocol proxyProtocol(BoostProcessor::netInstance(), &serverProtocol);
    serverProtocol.setProxyProtocol(&proxyProtocol);
    Net::Server::TcpServer innerServer(&serverProtocol, Reactor::instance(), BoostProcessor::netInstance());
    Net::Server::TcpServer proxyServer(&proxyProtocol, Reactor::instance(), BoostProcessor::netInstance());
    innerServer.start();
    proxyServer.start();

    unique_lock<mutex> lock(closedMutexM);
    while(!closed)
    {
        closedCondM.wait(lock);
    }

    LOG_DEBUG("TunnelServer stopped.");
    return 0;
}


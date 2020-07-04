#include "Processor.h"
#include "TcpServer.h"
#include "Reactor.h"
#include "TunnelClientProtocol.h"
#include "TunnelProxyClientProtocol.h"
#include "ProcessorSensor.h"
#include "Log.h"
#include "ConfigCenter.h"

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>

using namespace Processor;
using namespace Net::Reactor;
using namespace Net::Protocol;
using namespace std;

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

    TunnelClientProtocol clientProtocol(g_net_processor);

    unique_lock<mutex> lock(closedMutexM);
    while(!closed)
    {
        closedCondM.wait(lock);
    }

    LOG_DEBUG("TunnelClient stopped.");
    return 0;
}


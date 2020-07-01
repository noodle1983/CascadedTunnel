#include "BoostProcessor.h"
#include "TcpServer.h"
#include "Reactor.h"
#include "EchoProtocol.h"
#include "TelnetProtocol.h"
#include "TelnetCmdManager.h"
#include "ProcessorSensor.h"
#include "Log.h"

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>

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

	{
		Net::Protocol::TelnetCmdManager::registCmd("prcstat", Net::Protocol::ProcessorSensorSingleton::instance());
	}
    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    signal(SIGTERM, sig_stop);
    signal(SIGINT, sig_stop);
    evthread_use_pthreads();
    Processor::BoostProcessor processor("NetProcessor", 4);
    processor.start();
    Net::Reactor::Reactor reactor;
    Net::Protocol::EchoProtocol echoProtocol(&reactor, &processor);
    Net::Protocol::TelnetProtocol telnetProtocol(Processor::BoostProcessor::manInstance());
    Net::Server::TcpServer server(&echoProtocol, &reactor, &processor);
    Net::Server::TcpServer telnetServer(&telnetProtocol, &reactor, &processor);
    server.start();
    telnetServer.start();
    reactor.start();

    unique_lock<mutex> lock(closedMutexM);
    while(!closed)
    {
        closedCondM.wait(lock);
    }

    processor.stop();
    reactor.stop();
    LOG_DEBUG("EchoServer stopped.");
    return 0;
}


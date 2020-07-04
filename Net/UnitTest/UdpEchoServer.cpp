#include "Processor.h"
#include "UdpServer.h"
#include "Reactor.h"
#include "Protocol.h"
#include "Log.h"

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>

using namespace std;
using namespace nd;
using namespace nd;

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

class UdpEchoProtocol:public IUdpProtocol
{
public:
    UdpEchoProtocol(
        CppProcessor* theProcessor)
        : IUdpProtocol(theProcessor) 
    {
    }
    ~UdpEchoProtocol(){};

    void handleInput(UdpServerPtr theUdpServer)
    {
        UdpPacket packet;
        while (theUdpServer->getAPackage(&packet))
        {
            theUdpServer->sendAPackage(&packet);
        }
        return ;
    }
private:
};

int main()
{
    /*
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
    */

    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    signal(SIGTERM, sig_stop);
    signal(SIGINT, sig_stop);
    evthread_use_pthreads();
    CppProcessor processor(4);
    processor.start();
    Reactor reactor;
    UdpEchoProtocol echoProtocol(&processor);
    UdpServer *server = new UdpServer(&echoProtocol, &reactor, &processor);
    server->startAt(5555);
    reactor.start();

    unique_lock<mutex> lock(closedMutexM);
    while(!closed)
    {
        closedCondM.wait(lock);
    }

    server->close();
    processor.stop();
    reactor.stop();
    LOG_DEBUG("EchoServer stopped.");
    return 0;
}


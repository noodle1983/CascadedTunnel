#include "Processor.h"
#include "TcpServer.h"
#include "Reactor.h"
#include "EchoProtocol.h"
#include "TelnetProtocol.h"
#include "TelnetCmdManager.h"
#include "ProcessorSensor.h"
#include "Log.h"
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

	{
		TelnetCmdManager::registCmd("prcstat", ProcessorSensorSingleton::instance());
	}
    CppProcessor processor("NetProcessor", 4);
    processor.start();
    Reactor reactor;
    EchoProtocol echoProtocol(&processor);
    TelnetProtocol telnetProtocol(g_man_processor);
    TcpServer server(&echoProtocol, &reactor, &processor);
    TcpServer telnetServer(&telnetProtocol, &reactor, &processor);
    server.start();
    telnetServer.start();
    reactor.start();

    g_app->wait();

    processor.stop();
    reactor.stop();
    LOG_DEBUG("EchoServer stopped.");
    return 0;
}


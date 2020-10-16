#include <iostream>
#include <functional>

#include "Processor.h"
#include "Log.h"
#include "App.h"

using namespace std;
using namespace nd;

void on_timeout(void *theArg)
{
    std::cout << "time end" << endl;
    g_app->manualStop();
}

void say(CppProcessor* theProcessor)
{
    std::cout << "Hello, I will exit after 1 second" << std::endl;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    theProcessor->addLocalTimer(1, tv, on_timeout, NULL);
    std::cout << "time begin" << std::endl;
}

int main(int argc, char *argv[])
{
    g_app->parseAndInit(argc, argv);
    g_net_processor->process(1, new Job(bind(say, g_net_processor)));

    g_app->wait();
    g_net_processor->waitStop();

    g_app->fini();
    return 0;
}


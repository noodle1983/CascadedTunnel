#include <iostream>

#include <BoostProcessor.h>

using namespace std;
using namespace Processor;

void say(const char* theWords)
{
    std::cout << theWords << std::endl;
}

int main()
{
    g_net_processor->start();
    g_net_processor->process(1, new Job(bind(say, "Hello")));
    g_net_processor->waitStop();
    return 0;
}


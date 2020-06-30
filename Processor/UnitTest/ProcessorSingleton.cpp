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
    Processor::BoostProcessor::netInstance()->start();
    Processor::BoostProcessor::netInstance()->process(1, new Job(bind(say, "Hello")));
    sleep(1);
    return 0;
}


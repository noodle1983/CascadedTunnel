#include <iostream>
#include <functional>

#include "BoostProcessor.h"
#include "Log.h"

using namespace std;
using namespace Processor;

static int closed = false;
static mutex closedMutexM;
static condition_variable closedCondM;

void sig_stop(int sig)
{
    cout << "receive signal " << sig << ". stopping..." << endl;
    lock_guard<mutex> lock(closedMutexM);
    closed = true;
    closedCondM.notify_one();
}

void on_timeout(void *theArg)
{
    std::cout << "time end" << endl;
    sig_stop(2);
}

void say(Processor::BoostProcessor* theProcessor)
{
    std::cout << "Hello, I will exit after 1 second" << std::endl;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    theProcessor->addLocalTimer(1, tv, on_timeout, NULL);
    std::cout << "time begin" << std::endl;
}

int main()
{
    BoostProcessor processor(1);
    processor.start();
    processor.process(1, new Job(bind(say, &processor)));

    unique_lock<mutex> lock(closedMutexM);
    while(!closed)
    {
        closedCondM.wait(lock);
    }
    processor.waitStop();
    return 0;
}


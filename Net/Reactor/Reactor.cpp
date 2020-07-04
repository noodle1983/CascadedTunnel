#include "Reactor.h"
#include "Log.h"

#include <mutex>
#include <event.h>

using namespace nd;
using namespace std;

//-----------------------------------------------------------------------------

void on_heartbeat(int theFd, short theEvt, void *theArg)
{
    //DEBUG("reactor heartbeat.");
}

//-----------------------------------------------------------------------------

Reactor::Reactor()
{
    evtBaseM = event_base_new();
    heartbeatEventM = NULL;
}

//-----------------------------------------------------------------------------

Reactor::~Reactor()
{
    event_base_free(evtBaseM);
    if (heartbeatEventM)
    {
        delEvent(heartbeatEventM);
    }
}

//-----------------------------------------------------------------------------

static mutex reactorInstanceMutex;
static shared_ptr<Reactor> reactorInstanceReleaser;
Reactor* Reactor::reactorM = NULL;
Reactor* Reactor::instance()
{
    if (NULL == reactorM)
    {
        lock_guard<mutex> lock(reactorInstanceMutex);
        if (NULL == reactorM)
        {
            Reactor* reactor = new Reactor();
            reactorInstanceReleaser.reset(reactor);
            reactor->start();
            reactorM = reactor;
        }
    }
    return reactorM;
}

//-----------------------------------------------------------------------------

void Reactor::start()
{
    heartbeatEventM = event_new(evtBaseM, -1, EV_PERSIST, on_heartbeat, this);
    struct timeval tv;
    tv.tv_sec = 5;  //5 seconds
    tv.tv_usec = 0;
    event_add(heartbeatEventM, &tv);
    threadM = thread(&Reactor::dispatchLoop, this);
}

//-----------------------------------------------------------------------------

void Reactor::dispatchLoop()
{
    LOG_DEBUG("enter event dispatch.");
    int ret = event_base_dispatch(evtBaseM);
    if (-1 == ret)
    {
        LOG_ERROR("exit event dispatch with error.");
    }
    else
    {
        LOG_DEBUG("exit event dispatch.");
    }

}

//-----------------------------------------------------------------------------

void Reactor::stop()
{
    event_base_loopexit(evtBaseM, NULL);
    if (threadM.joinable()){threadM.join();}
}




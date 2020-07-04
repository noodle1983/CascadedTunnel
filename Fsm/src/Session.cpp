#include "Session.h"
#include "Log.h"
#include "Processor.h"
#include "Reactor.h"

#include <utility>

using namespace std;

#ifdef DEBUG
#include <thread>
#include <assert.h>
extern thread_local unsigned g_threadGroupTotal;
extern thread_local unsigned g_threadGroupIndex;
static const thread::id default_thread_id;
#endif

using namespace Fsm;
typedef std::pair<Session*, unsigned char> TimerPair;

//-----------------------------------------------------------------------------

Session::Session(
        FiniteStateMachine* theFsm, 
        const uint64_t theSessionId)
    : isInitializedM(false)
    , curStateIdM(theFsm->getFirstStateId())
    , endStateIdM(theFsm->getLastStateId())
    , timerIdM(0)
    , fsmM(theFsm)
    , fsmTimeoutEvtM(NULL)
    , sessionIdM(theSessionId)
{
}

//-----------------------------------------------------------------------------

Session::Session()
    : isInitializedM(false)
    , curStateIdM(0)
    , endStateIdM(0)
    , timerIdM(0)
    , fsmM(NULL)
    , fsmTimeoutEvtM(NULL)
    , sessionIdM(0)
{
}

//-----------------------------------------------------------------------------

void Session::init(
        FiniteStateMachine* theFsm, 
        const uint64_t theSessionId)
{
    fsmM = theFsm;
    sessionIdM = theSessionId;
    isInitializedM = false;
    curStateIdM = fsmM->getFirstStateId();
    endStateIdM = fsmM->getLastStateId();
    timerIdM = 0;
    if (fsmTimeoutEvtM)
    {
        Processor::BoostProcessor::fsmInstance()->cancelLocalTimer(
                sessionIdM, fsmTimeoutEvtM);
    }
    fsmTimeoutEvtM = NULL;
}

//-----------------------------------------------------------------------------

Session::~Session()
{
}

//-----------------------------------------------------------------------------

int Session::asynHandleEvent(const int theEventId)
{
    Processor::BoostProcessor::fsmInstance()->PROCESS(sessionIdM,
        &Session::handleEvent, this, theEventId);
    return 0;
}

//-----------------------------------------------------------------------------

void Session::handleEvent(const int theEventId)
{
#ifdef DEBUG
    unsigned threadCount = g_threadGroupTotal;
    unsigned threadIndex = g_threadGroupIndex;
    if ((sessionIdM%threadCount) != threadIndex)
    {
        LOG_FATAL("job is handled in wrong thread:" << threadIndex 
                << ", count:" << threadCount
                << ", id:" << sessionIdM);
        assert(false);
    }
    

    if (default_thread_id == tidM)
    {
        tidM = this_thread::get_id();
    }
    else if (tidM != this_thread::get_id())
    {
        LOG_FATAL("tid not match pre:" << tidM << "-now:" << this_thread::get_id());
        assert(false);
    }
#endif
    if (!isInitializedM)
    {
        //the first state's entry function
        const int curStateId = curStateIdM;
        State& curState = getCurState();
        ActionList& actionList = curState.getActionList(ENTRY_EVT);
        ActionList::iterator it = actionList.begin();
        if (it != actionList.end())
        {
            LOG_DEBUG(getSessionName() 
                    << "[" << getSessionId() << "] handleEvent("
                    << getEventName(ENTRY_EVT) << ")");
            for (; it != actionList.end(); it++)
            {
                if (curStateId != curStateIdM)
                {
                    LOG_DEBUG("state changed, ignore rest action for event:" << theEventId);
                    break;
                }
                (*it)(this);
            }
        }
        isInitializedM = true;
    }

    LOG_DEBUG(getSessionName() 
            << "[" << getSessionId() << "] handleEvent("
            << getEventName(theEventId) << ")");
    State& curState = getCurState();
    ActionList& actionList = curState.getActionList(theEventId);
    if (actionList.empty())
    {
        LOG_ERROR(getSessionName()
                << "[" << getSessionId() << "]"
                << " the Event " << theEventId << " is not defined"
                << " under state:" << curState.getName());
        changeState(this, endStateIdM);
        return ;
    }

    const int curStateId = curStateIdM;
    ActionList::iterator it = actionList.begin();
    for (; it != actionList.end(); it++)
    {
        if (curStateId != curStateIdM)
        {
            LOG_DEBUG(getSessionName()
                    << " state changed, ignore rest action for event:" << theEventId);
            break;
        }
        (*it)(this);
    }
    return ;
}

//-----------------------------------------------------------------------------

State& Session::toNextState(const int theNextStateId)
{
    const std::string& preStateName = fsmM->getState(curStateIdM).getName();

    curStateIdM = theNextStateId;
    State& nextState = fsmM->getState(curStateIdM);

    LOG_DEBUG( getSessionName() << "[" << sessionIdM << "] " 
            << preStateName << " -> " << nextState.getName());

    return nextState;
}

//-----------------------------------------------------------------------------

void onFsmTimeOut(void *theArg)
{
    Session* session = (Session*)theArg;
    session->handleTimeout();
}

//-----------------------------------------------------------------------------

void Session::handleTimeout()
{
#ifdef DEBUG
    unsigned threadCount = g_threadGroupTotal;
    unsigned threadIndex = g_threadGroupIndex;
    if ((sessionIdM%threadCount) != threadIndex)
    {
        LOG_FATAL("job is handled in wrong thread:" << threadIndex 
                << ", count:" << threadCount
                << ", id:" << sessionIdM);
        assert(false);
    }
    

    if (default_thread_id == tidM)
    {
        tidM = this_thread::get_id();
    }
    else if (tidM != this_thread::get_id())
    {
        LOG_FATAL("tid not match pre:" << tidM << "-now:" << this_thread::get_id());
        assert(false);
    }
#endif
    fsmTimeoutEvtM = NULL; 
    handleEvent(TIMEOUT_EVT);
}

//-----------------------------------------------------------------------------

void Session::newTimer(const long long theUsec)
{
    if (fsmTimeoutEvtM)
    {
        cancelTimer();
    }
    struct timeval tv;
    tv.tv_sec = theUsec/1000000;
    tv.tv_usec = theUsec%1000000;
    fsmTimeoutEvtM = Processor::BoostProcessor::fsmInstance()->addLocalTimer(
                sessionIdM, tv, onFsmTimeOut, (void*)this);
}

//-----------------------------------------------------------------------------

void Session::cancelTimer()
{
    if (fsmTimeoutEvtM)
    {
        Processor::BoostProcessor::fsmInstance()->cancelLocalTimer(
                sessionIdM, fsmTimeoutEvtM);
        fsmTimeoutEvtM = NULL;
    }
}

//-----------------------------------------------------------------------------


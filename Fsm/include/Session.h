#ifndef SESSION_H
#define SESSION_H

#include "Fsm.h"
#include "State.h"
#include "min_heap.h"

#include <stdint.h>
#include <string>
#include <thread>

namespace Processor
{
    class BoostProcessor;
}

namespace Fsm
{
    class Session
    {
    public:
        Session(
                FiniteStateMachine* theFsm, 
                const uint64_t theSessionId);
        Session();
        void init(
                FiniteStateMachine* theFsm, 
                const uint64_t theSessionId);
        virtual ~Session();


        State& toNextState(const int theNextStateId);
		int asynHandleEvent(const int theEventId);
        void handleEvent(const int theEventId);
        void newTimer(const long long theUsec);
        void handleTimeout();
        void cancelTimer();


        const uint64_t getSessionId()
        {
            return sessionIdM;
        }

        virtual const char* getSessionName()
        {
            return "Session";
        }

        const std::string& getEventName(const int theEventName)
        {
            return fsmM->getEventName(theEventName);
        }

        inline State& getEndState()
        {
            return fsmM->getState(endStateIdM);
        }

        inline State& getCurState()
        {
            return fsmM->getState(curStateIdM);
        }

    protected:
        bool isInitializedM;
        unsigned char curStateIdM;
        unsigned char endStateIdM;
        unsigned timerIdM;
        FiniteStateMachine* fsmM;

        min_heap_item_t* fsmTimeoutEvtM;
        uint64_t sessionIdM;

#ifdef DEBUG
        std::thread::id tidM;
#endif

    };
}

#endif /* SESSION_H */

#ifndef ACTION_H
#define ACTION_H

#include <boost/function.hpp>
#include <boost/bind/bind.hpp>
using namespace boost::placeholders;

#define CHANGE_STATE(theStateId)  boost::bind(Fsm::changeState,   _1, (theStateId))
#define GEN_EVT(theEventId)       boost::bind(Fsm::generateEvent, _1, (theEventId))
#define NEW_TIMER(theUsec)        boost::bind(Fsm::newTimer,      _1, (theUsec))
#define NEW_FUNC_TIMER(theGettor) boost::bind(Fsm::newFuncTimer,  _1, Fsm::TimerGettor(theGettor))
#define CANCEL_TIMER()            (&Fsm::cancelTimer)
#define DELETE_SESSION()          (&Fsm::deleteSession)
#define IGNORE_EVT()              (&Fsm::ignoreEvent)

namespace Fsm
{
    class Session;
    typedef boost::function<void (Fsm::Session *)> Action;
    typedef boost::function<long long ()> TimerGettor;

    int changeState(
            Fsm::Session* theSession,
            const int theNextStateId);

    int generateEvent(
            Fsm::Session* theSession,
            const int theEventId);

    int ignoreEvent(
            Fsm::Session* theSession);

    int newTimer(
            Fsm::Session* theSession,
            const long long theUsec);

    int newFuncTimer(
            Fsm::Session* theSession,
            TimerGettor theTimerGettor);

    int cancelTimer(
            Fsm::Session* theSession);

    int deleteSession(
            Fsm::Session* theSession);
}

#endif /* ACTION_H */

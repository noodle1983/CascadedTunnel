#include "Fsm.h"
#include "Action.h"
#include "State.h"
#include "Session.h"
#include "Reactor.h"
#include <iostream>
#include <functional>
#include <unistd.h>
using namespace std;

int testEnter(Fsm::Session* theSession)
{
    cout << "enter" << endl;
    return 0;
}
int testExit(Fsm::Session* theSession)
{
    cout << "exit" << endl;
    return 0;
}

enum MyState
{
    INIT_STATE = 1,
    TIME_STATE = 2,
    END_STATE  = 3
};

enum MyEvent
{
    START_EVT = 0
};

int main()
{
    Fsm::FiniteStateMachine fsm;
    fsm += FSM_STATE(INIT_STATE);
    fsm +=      FSM_EVENT(Fsm::ENTRY_EVT,  &testEnter);
    fsm +=      FSM_EVENT(START_EVT,  CHANGE_STATE(TIME_STATE));
    fsm +=      FSM_EVENT(Fsm::EXIT_EVT,   &testExit);

    fsm += FSM_STATE(TIME_STATE);
    fsm +=      FSM_EVENT(Fsm::ENTRY_EVT,   NEW_TIMER(1 * 1000000));
    fsm +=      FSM_EVENT(Fsm::TIMEOUT_EVT, CHANGE_STATE(END_STATE));
    fsm +=      FSM_EVENT(Fsm::EXIT_EVT,    CANCEL_TIMER());

    fsm += FSM_STATE(END_STATE);
    fsm +=      FSM_EVENT(Fsm::ENTRY_EVT,  &testEnter);
    fsm +=      FSM_EVENT(Fsm::EXIT_EVT,   &testExit);
    cout << "fsm,initstate:" << fsm.getFirstStateId() << endl;
    cout << "fsm,endstate:" << fsm.getLastStateId() << endl;

    int * i = new int;
    Fsm::Session session;
    session.init(&fsm, 0);
    session.asynHandleEvent(START_EVT);
    sleep(3);
    return 0;
}


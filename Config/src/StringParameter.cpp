#include "StringParameter.h"
#include "Log.h"

#include <stdio.h>

using namespace Config;
using namespace std;

//-----------------------------------------------------------------------------

StringParameter::StringParameter(const std::string& theName)
    : nameM(theName)
{
}

//-----------------------------------------------------------------------------

StringParameter::StringParameter(const StringParameter& theStringParam)
    : nameM(theStringParam._getName())
    , valueM(theStringParam._getValue())
{
}
//-----------------------------------------------------------------------------

StringParameter::~StringParameter()
{}

//-----------------------------------------------------------------------------

const std::string StringParameter::get()
{
    lock_guard<mutex> lock(valueMutexM);
    return valueM;
}

//-----------------------------------------------------------------------------

int StringParameter::set(const std::string& theValue)
{
    if (theValue == valueM)
        return 0;

    {
        lock_guard<mutex> lock(valueMutexM);
        valueM = theValue;
    }
    {
        lock_guard<mutex> lock(watcherMutexM);
        for (WatcherMap::iterator it = changesWatchersM.begin();
                it != changesWatchersM.end(); it++)
        {
            (it->second)(theValue);
        }
    }
    CFG_DEBUG("set config[" << nameM << "=" << theValue << "]");
    return 0;
}

//-----------------------------------------------------------------------------

void StringParameter::registerWatcher(void *theKey, Watcher& theWatcher)
{
    lock_guard<mutex> lock(watcherMutexM);
    changesWatchersM[theKey] = theWatcher;
}

//-----------------------------------------------------------------------------

void StringParameter::unregisterWatcher(void *theKey)
{
    lock_guard<mutex> lock(watcherMutexM);
    changesWatchersM.erase(theKey);
}

//-----------------------------------------------------------------------------





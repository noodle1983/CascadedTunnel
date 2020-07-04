#include "IntParameter.h"
#include "Log.h"

#include <stdio.h>

using namespace nd;
using namespace std;

//-----------------------------------------------------------------------------

IntParameter::IntParameter(const std::string& theName)
    : nameM(theName)
    , checkRangeM(false)
    , minValueM(0)
    , maxValueM(0)
    , valueM(0)
{
}

//-----------------------------------------------------------------------------

IntParameter::IntParameter(const IntParameter& theIntParam)
    : nameM(theIntParam._getName())
    , checkRangeM(theIntParam._getCheckRange())
    , minValueM(theIntParam._getMinValue())
    , maxValueM(theIntParam._getMaxValue())
    , valueM(theIntParam._getValue())
{
}
//-----------------------------------------------------------------------------

IntParameter::~IntParameter()
{}

//-----------------------------------------------------------------------------

int IntParameter::get()
{
    lock_guard<mutex> lock(valueMutexM);
    return valueM;
}

//-----------------------------------------------------------------------------

int IntParameter::set(const std::string& theValue)
{
    int value = 0;
    if (0 == sscanf(theValue.c_str(), "%d", &value))
    {
        CFG_ERROR(nameM << "'s value " << theValue << " is invalid.");
        return -1;
    }
    return set(value);
}

//-----------------------------------------------------------------------------

int IntParameter::set(const int theValue)
{
    if (theValue == valueM)
        return 0;

    bool notifyWatcher = false;

    {
        lock_guard<mutex> lock(valueMutexM);
        if (!checkRangeM 
                || (checkRangeM && theValue >= minValueM && theValue <= maxValueM))
        {
            {
                valueM = theValue;
            }
            notifyWatcher = true;
        }
        else
        {
            CFG_ERROR("invalid value: "<< theValue
                    << " for " << nameM 
                    << "(" << minValueM << "-" << maxValueM << ")");        
            return -1;
        }
    }
    if (notifyWatcher)
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

void IntParameter::setRange(const std::string& theRange)
{
    if (theRange.empty())
        return;

    int minValue = 0;
    int maxValue = 0;
    if (0 == sscanf(theRange.c_str(), "%d-%d", &minValue, &maxValue))
    {
        CFG_ERROR(nameM << "'s range " << theRange << " is invalid.");
        return;
    }
    if (minValue < maxValue)
    {
        return setRange(minValue, maxValue);
    }
    else
    {
        return setRange(maxValue, minValue);
    }

}

//-----------------------------------------------------------------------------

void IntParameter::setRange(const int theMin, const int theMax)
{
    lock_guard<mutex> lock(valueMutexM);
    checkRangeM = true;
    minValueM = theMin;
    maxValueM = theMax;
}

//-----------------------------------------------------------------------------

void IntParameter::registerWatcher(void* theKey, Watcher& theWatcher)
{
    lock_guard<mutex> lock(watcherMutexM);
    changesWatchersM[theKey] = theWatcher;
}

//-----------------------------------------------------------------------------

void IntParameter::unregisterWatcher(void* theKey)
{
    lock_guard<mutex> lock(watcherMutexM);
    changesWatchersM.erase(theKey);
}

//-----------------------------------------------------------------------------



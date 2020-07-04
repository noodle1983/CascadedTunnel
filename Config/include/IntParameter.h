#ifndef INTPARAMETER_H
#define INTPARAMETER_H

#include <functional>
#include <mutex>
#include <map>
#include <string>

namespace nd
{
    class IntParameter
    {
    public:
        IntParameter(const std::string& theName);
        IntParameter(const IntParameter& theIntParam);
        ~IntParameter();

        typedef std::function<void (const int)> Watcher;
        typedef std::map<void*, Watcher> WatcherMap;

        int get();
        int set(const std::string& theValue);
        int set(const int theValue);
        void setRange(const std::string& theRange);
        void setRange(const int theMin, const int theMax);
        void registerWatcher(void* theKey, Watcher& theWatcher);
        void unregisterWatcher(void* theKey);

        const std::string& _getName()const {return nameM;}
        bool  _getCheckRange()const {return checkRangeM;}
        int  _getMinValue()const {return minValueM;}
        int  _getMaxValue()const {return maxValueM;}
        int  _getValue()const {return valueM;}
    private:
        const std::string nameM;
        bool checkRangeM;
        int minValueM;
        int maxValueM;
        int valueM;
        WatcherMap changesWatchersM;
        std::mutex valueMutexM;
        std::mutex watcherMutexM;
    };
}
#endif /* INTPARAMETER_H */


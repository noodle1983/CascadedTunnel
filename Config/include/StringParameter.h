#ifndef STRINGPARAMETER_H
#define STRINGPARAMETER_H

#include <functional>
#include <mutex>
#include <map>
#include <string>

namespace nd
{
    class StringParameter
    {
    public:
        StringParameter(const std::string& theName);
        StringParameter(const StringParameter& theStringParam);
        ~StringParameter();

        typedef std::function<void (const std::string&)> Watcher;
        typedef std::map<void*, Watcher> WatcherMap;

        const std::string get();
        int set(const std::string& theValue);
        void registerWatcher(void *theKey, Watcher& theWatcher);
        void unregisterWatcher(void *theKey);

        const std::string& _getName()const {return nameM;}
        const std::string& _getValue()const {return valueM;}
    private:
        const std::string nameM;
        std::string valueM;
        WatcherMap changesWatchersM;
        std::mutex valueMutexM;
        std::mutex watcherMutexM;
    };
}
#endif /* STRINGPARAMETER_H */


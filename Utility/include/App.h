#ifndef APP_H
#define APP_H

#include "Processor.h"
#include "Singleton.hpp"

namespace nd
{
    class App 
    {
    public:
        friend class DesignPattern::Singleton<App>;
        ~App();

        void setRunInBackground();
        void setDumpWhenCrash();
        static void wait();

    private:
        App();
        static bool dumpCallback(
            const char* dump_path,
            const char* minidump_id,
            void* context,
            bool succeeded);
        static void sigStop(int sig);

    private:
        google_breakpad::ExceptionHandler* crashDumpHandlerM;
        static int closedM;
        static boost::mutex closedMutexM;
        static boost::condition_variable closedCondM;
    };

    typedef DesignPattern::Singleton<App> AppSingleton;
}

#endif /* APP_H */


#ifndef APP_H
#define APP_H

#include "Singleton.hpp"
#include <condition_variable>
#include <mutex>
#include <string>

namespace nd
{
    class App 
    {
    public:
        friend class DesignPattern::Singleton<App>;
        virtual ~App() = default;

        void parseAndInit(int argc, char *argv[]);
        void wait();
        //void setDumpWhenCrash();
        
        bool runInBackground(){return isBackgroundM;} 

    private:
        App();
        void init();
        void setRunInBackground();
        static bool dumpCallback(
            const char* dump_path,
            const char* minidump_id,
            void* context,
            bool succeeded);
        static void sigStop(int sig);

    private:
        //google_breakpad::ExceptionHandler* crashDumpHandlerM;
        bool isBackgroundM;
        std::string cfgFileM;

        static int closedM;
        static std::mutex closedMutexM;
        static std::condition_variable closedCondM;
    };

    typedef DesignPattern::Singleton<App> AppSingleton;
}
#define g_app nd::AppSingleton::instance()

#endif /* APP_H */


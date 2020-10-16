#include "App.h"
#include "Log.h"
#include "DirCreator.h"
#include "Processor.h"
#include "ConfigCenter.h"

#include <event.h>
#include <event2/thread.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <filesystem>
#include <chrono>

using namespace nd;
using namespace std;
namespace fs = std::filesystem;

int App::closedM = false;
std::mutex App::closedMutexM;
std::condition_variable App::closedCondM;

//-----------------------------------------------------------------------------
//    : crashDumpHandlerM(NULL)

App::App()
    : isBackgroundM(false)
    , cfgFileM("config.json")
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    signal(SIGTERM, App::sigStop);
    signal(SIGINT, App::sigStop);
}

//-----------------------------------------------------------------------------

//App::~App()
//{
//    if (crashDumpHandlerM)
//    {
//        delete crashDumpHandlerM;
//    }
//}

//-----------------------------------------------------------------------------

void App::parseAndInit(int argc, char *argv[])
{
    const int STATE_CFG_FILE = 1;

    int state = 0;
    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0 && state == 0){
            isBackgroundM = true; 
            continue;
        }
        if (strcmp(argv[i], "-c") == 0 && state == 0){
            state = STATE_CFG_FILE;
            continue;
        }
        if (strcmp(argv[i], "-h") == 0){
            cout << "help: " << argv[0] << " [-b] [-c config_file.xml]" << endl; 
            exit(0);
        }
        if (state == STATE_CFG_FILE)
        {
            cfgFileM = argv[i];
            if (!fs::exists(cfgFileM)) {
                cerr << "config file: " << argv[i] << " not exist!" << endl; 
                exit(-1);
            }
        }
    }
    init();
}

//-----------------------------------------------------------------------------

void App::init()
{
    setRunInBackground();

    // init g_xxx
    evthread_use_pthreads();
    g_io_processor; // init before logs and others

    if (fs::exists(cfgFileM)) {
        if (0 != g_cfg->load(cfgFileM)) {
            CFG_FATAL("fail to load config file:" << cfgFileM << "!");
            exit(0);
        }
        CFG_DEBUG("loaded config file:" << cfgFileM);
    } else {
        CFG_WARN("run without config file!");
    }
}

//-----------------------------------------------------------------------------

void App::fini()
{
    LOG_DEBUG("process stop...");
    g_io_processor->waitStop();
    g_logger->fini();
    g_cfg_logger->fini();
}

//-----------------------------------------------------------------------------

void App::setRunInBackground()
{
    if (isBackgroundM)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            CFG_FATAL("failed to fork!");
            exit(-1);
        }
        else if (pid > 0)
        {
            CFG_FATAL("server start with pid:" << pid);
            exit(0);
        }
        setsid();
    }
}

//-----------------------------------------------------------------------------

//void App::setDumpWhenCrash()
//{
//    if (NULL == crashDumpHandlerM)
//    {
//        createDir("./CrashDump");
//        crashDumpHandlerM = new google_breakpad::ExceptionHandler("./CrashDump", NULL, App::dumpCallback, NULL, true);
//    }
//}

//-----------------------------------------------------------------------------

void App::wait()
{
    unique_lock<mutex> lock(closedMutexM);
    while(!closedM)
    {
        closedCondM.wait(lock);
    }
    fini();
}

//-----------------------------------------------------------------------------

void App::sleep(size_t ms)
{
    this_thread::sleep_for(chrono::milliseconds(ms));
}

//-----------------------------------------------------------------------------

bool App::dumpCallback(const char* dump_path,
                        const char* minidump_id,
                        void* context,
                        bool succeeded)
{
    LOG_WARN("Crash Dump: " << dump_path << "/" << minidump_id << ".dmp");
    return succeeded;
}

//-----------------------------------------------------------------------------

void App::sigStop(int sig)
{
    LOG_DEBUG("receive signal " << sig << ". stopping...");
    lock_guard<mutex> lock(closedMutexM);
    closedM = true;
    closedCondM.notify_one();
}


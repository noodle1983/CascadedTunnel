#include "FileWatcher.h"
#include "Log.h"
#include "Reactor.h"
#include "Processor.h"

#include <sys/stat.h>
#include <unistd.h>

using namespace nd;

//-----------------------------------------------------------------------------

static void onFilewatcherTimeout(int theFd, short theEvt, void *theArg)
{
    FileWatcher* theWatcher = (FileWatcher*) theArg;
    g_man_processor->PROCESS(
            0, 
            &FileWatcher::checkFile, theWatcher);
}

//-----------------------------------------------------------------------------

FileWatcher::FileWatcher(
        const std::string& thePath, 
        unsigned theSec, 
        FileWatcherCallback theCallback)
    : filePathM(thePath)
    , secM(theSec)
    , callbackM(theCallback)
{
    struct stat fileStat;
    if (0 == stat(filePathM.c_str(), &fileStat))
    {
        lastModTimeM = fileStat.st_mtime;
    }
    else
    {
        CFG_WARN("failed to stat file:" << filePathM
                << ". errno:" << errno);
        lastModTimeM = 0;
    }

    timerEventM = g_reactor->newTimer(
                            onFilewatcherTimeout, 
                            this); 
    struct timeval tv;
    tv.tv_sec = (secM == 0 ? 5 : secM); 
    tv.tv_usec = 0;
    event_add(timerEventM, &tv);
}

//-----------------------------------------------------------------------------

FileWatcher::~FileWatcher()
{
    if (timerEventM)
    {
        g_reactor->delEvent(timerEventM);
        timerEventM = NULL;
    }
}

//-----------------------------------------------------------------------------

void FileWatcher::checkFile()
{
    struct stat fileStat;
    if (0 == stat(filePathM.c_str(), &fileStat))
    {
        if (lastModTimeM != fileStat.st_mtime)
        {
            g_man_processor->PROCESS(
                    0, callbackM, filePathM);
            lastModTimeM = fileStat.st_mtime;
            CFG_DEBUG("reload file:" << filePathM);
        }
        struct timeval tv;
        tv.tv_sec = (secM == 0 ? 5 : secM); 
        tv.tv_usec = 0;
        event_add(timerEventM, &tv);
        return ;
    }
    else
    {
        struct timeval tv;
        tv.tv_sec = (secM == 0 ? 5 : secM); 
        tv.tv_usec = 0;
        event_add(timerEventM, &tv);
        return ;
    }

    return ;
}


#include "FileWriter.h"

#include <unistd.h>
#include <Log.h>
#include <ConfigCenter.h>
#include <DirCreator.h>
#include <string.h>

using namespace Utility;
using namespace Config;

//-----------------------------------------------------------------------------

FileWriter::FileWriter(
        Processor::BoostProcessor* theProcessor,
        const std::string& theModelName, 
        const int64_t theIndex, 
        const std::string& theHeaderLine)
    : headerLineM(theHeaderLine)
    , modelNameM(theModelName)
    , indexM(theIndex)
    , switchTimeM(0)
    , curWriteTimeM(0)
    , curTimerTimeM(0)
    , curSizeM(0)
    , processorM(theProcessor)
    , timerHandlerM(NULL)
{
    switchTimeM = ConfigCenter::instance()->get(modelNameM+ ".swtTime", 3600);
    switchSizeM = ConfigCenter::instance()->get(modelNameM+ ".swtSize", 100);
    switchSizeM *= 1024 * 1024;
    outDirM = ConfigCenter::instance()->get(modelNameM+ ".outDir", "./OutputFiles");
    createDir(outDirM);
}

//-----------------------------------------------------------------------------

FileWriter::FileWriter(
        const std::string& theModelName, 
        const int64_t theIndex,
        const std::string& theHeaderLine)
    : headerLineM(theHeaderLine)
    , modelNameM(theModelName)
    , indexM(theIndex)
    , switchTimeM(0)
    , curWriteTimeM(0)
    , curTimerTimeM(0)
    , curSizeM(0)
    , processorM(NULL)
    , timerHandlerM(NULL)
{
    switchTimeM = ConfigCenter::instance()->get(modelNameM+ ".swtTime", 3600);
    switchSizeM = ConfigCenter::instance()->get(modelNameM+ ".swtSize", 100);
    switchSizeM *= 1024 * 1024;
    outDirM = ConfigCenter::instance()->get(modelNameM+ ".outDir", "./OutputFiles");
    createDir(outDirM);
}

//-----------------------------------------------------------------------------

FileWriter::~FileWriter()
{
    closeFile();
    if (timerHandlerM)
    {
        processorM->cancelLocalTimer(indexM, timerHandlerM);
    }
}

//-----------------------------------------------------------------------------

void FileWriter::write(const std::string& theContent, const time_t theTime)
{
    if (NULL == timerHandlerM)
    {
        startTimer(theTime);
    }
    curSizeM += theContent.length();
    switchFile(theTime);
    fileStreamM << theContent;
}

//-----------------------------------------------------------------------------

void FileWriter::_write(const std::string* theContent, const time_t theTime)
{
    write(*theContent, theTime);
    delete theContent;
}

//-----------------------------------------------------------------------------



void FileWriter::startTimer(const time_t &theTime)
{
    if (NULL == processorM)
        return;
    
    if (NULL != timerHandlerM)
    {
        processorM->cancelLocalTimer(
            indexM, timerHandlerM);
    }

    time_t now = time((time_t*)NULL);
    struct timeval tv;
    //delay 60s
    tv.tv_sec = switchTimeM - now % switchTimeM + 60;
    tv.tv_usec = 0;
    timerHandlerM = processorM->addLocalTimer(
            indexM, tv, FileWriter::onTimeout, (void*)this);

    curTimerTimeM = (theTime / switchTimeM * switchTimeM + switchTimeM);
}

//-----------------------------------------------------------------------------

void FileWriter::onTimeout(void *theArg)
{
    FileWriter* writer = (FileWriter*) theArg;
    writer->timerHandlerM = NULL;
    writer->switchFile(writer->curTimerTimeM);
    writer->startTimer(writer->curTimerTimeM);
}

//-----------------------------------------------------------------------------


void FileWriter::switchFile(const time_t& theTime)
{
    time_t now = time((time_t*)NULL);
    time_t tableTimestamp = theTime;
    unsigned newWriteTime = tableTimestamp / switchTimeM * switchTimeM;
    if (newWriteTime > curWriteTimeM || !fileStreamM.is_open()
            || (switchSizeM > 0 && curSizeM > switchSizeM))
    {
        closeFile();

        curWriteTimeM = newWriteTime;
        struct tm tmWriteTime;
        localtime_r(&now, &tmWriteTime);
        struct tm tmTableTime;
        localtime_r(&tableTimestamp, &tmTableTime);

        char filenameBuffer[256] = {0};

        memset(filenameBuffer, 0 , sizeof(filenameBuffer));
        snprintf(filenameBuffer, sizeof(filenameBuffer), 
                "%04d%02d%02d%02d%02d%02d",
                tmWriteTime.tm_year + 1900,
                tmWriteTime.tm_mon + 1,
                tmWriteTime.tm_mday,
                tmWriteTime.tm_hour,
                tmWriteTime.tm_min,
                tmWriteTime.tm_sec
                );
        beginTimeStrM = filenameBuffer;

        memset(filenameBuffer, 0 , sizeof(filenameBuffer));
        snprintf(filenameBuffer, sizeof(filenameBuffer), 
                "%s_%04d%02d%02d%02d",
                modelNameM.c_str(),
                tmTableTime.tm_year + 1900,
                tmTableTime.tm_mon + 1,
                tmTableTime.tm_mday,
                tmTableTime.tm_hour
                );
        dbTableNameM = filenameBuffer;

        memset(filenameBuffer, 0 , sizeof(filenameBuffer));
        snprintf(filenameBuffer, sizeof(filenameBuffer), 
                "%s/%s_%02d__%s.txt",
                outDirM.c_str(),
                beginTimeStrM.c_str(),
                indexM,
                dbTableNameM.c_str()
                );
        tmpFilePathM = filenameBuffer;
        fileStreamM.open(filenameBuffer, std::ios_base::out|std::ios_base::app);
        curSizeM = headerLineM.length();;
        fileStreamM << headerLineM;
    }
}

//-----------------------------------------------------------------------------

void FileWriter::closeFile()
{
    curSizeM = 0;
    if (fileStreamM.is_open())
    {
        fileStreamM.flush();
        fileStreamM.close();
        std::string& fromFilePath = tmpFilePathM;

        time_t now = time((time_t*)NULL);
        struct tm tmWriteTime;
        localtime_r(&now, &tmWriteTime);
        char filenameBuffer[256] = {0};
        snprintf(filenameBuffer, sizeof(filenameBuffer), 
                "%04d%02d%02d%02d%02d%02d",
                tmWriteTime.tm_year + 1900,
                tmWriteTime.tm_mon + 1,
                tmWriteTime.tm_mday,
                tmWriteTime.tm_hour,
                tmWriteTime.tm_min,
                tmWriteTime.tm_sec
                );
        std::string endTimeStr = filenameBuffer;

        memset(filenameBuffer, 0 , sizeof(filenameBuffer));
        snprintf(filenameBuffer, sizeof(filenameBuffer), 
                "%s/%s_%s_%02d__%s.ss7",
                outDirM.c_str(),
                beginTimeStrM.c_str(),
                endTimeStr.c_str(),
                indexM,
                dbTableNameM.c_str()
                );
        if(0 != link(fromFilePath.c_str(), filenameBuffer))
        {
            LOG_ERROR("fail to rename the file:" << fromFilePath
                    << ", errno:" << errno);
        }
        else
        {
            unlink(fromFilePath.c_str());
        }
    }
}

//-----------------------------------------------------------------------------


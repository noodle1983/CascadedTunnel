#include "AsynFileWriter.h"

#include <unistd.h>
#include <Log.h>
#include <ConfigCenter.h>
#include <DirCreator.h>

using namespace nd;

//-----------------------------------------------------------------------------

AsynFileWriter::AsynFileWriter(
        const std::string& theModelName, 
        const std::string& theHeaderLine)
{
    ioThreadCntM = ConfigCenter::instance()->get("prc.ioTno", 1);
    for(unsigned i = 0; i < ioThreadCntM; i++)
    {   
        fileWriterVectorM.push_back(new FileWriter(
                    g_io_processor,
                    theModelName, 
                    i,
                    theHeaderLine));
    }
}

//-----------------------------------------------------------------------------

AsynFileWriter::~AsynFileWriter()
{
    for(unsigned i = 0; i < ioThreadCntM; i++)
    {
        delete fileWriterVectorM[i];
    }
    fileWriterVectorM.clear();
}

//-----------------------------------------------------------------------------

void AsynFileWriter::write(const uint64_t theId, const std::string& theContent, const time_t theTime)
{
    FileWriter* fileWriter = fileWriterVectorM[theId%ioThreadCntM];
    const std::string* content = new std::string(theContent);
    g_io_processor->PROCESS(theId, &FileWriter::_write,
            fileWriter, content, theTime);
}

//-----------------------------------------------------------------------------

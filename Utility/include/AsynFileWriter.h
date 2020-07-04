#ifndef ASYNFILEWRITER_H
#define ASYNFILEWRITER_H

#include "FileWriter.h"
#include <string>
#include <vector>
#include <stdint.h>

namespace nd
{
    class AsynFileWriter
    {
    public:
        AsynFileWriter(
                const std::string& theModelName, 
                const std::string& theHeaderLine = "");
        ~AsynFileWriter();

        void write(const uint64_t theId, const std::string& theContent, const time_t theTime);
        void _write(
                FileWriter* theFileWriter, 
                const std::string* theContent, 
                const time_t theTime);

    private:
        unsigned ioThreadCntM;
        std::vector<FileWriter*> fileWriterVectorM;
    };

}

#endif /* ASYNFILEWRITER_H */


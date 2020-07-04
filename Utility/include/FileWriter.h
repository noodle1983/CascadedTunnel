#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "Processor.h"
#include <fstream>
#include <string>
#include <stdint.h>
#include <unistd.h>

namespace nd
{
    class FileWriter
    {
    public:
        FileWriter(
                CppProcessor* theProcessor,
                const std::string& theModelName, 
                const int64_t theIndex, 
                const std::string& theHeaderLine = "");
        FileWriter(
                const std::string& theModelName, 
                const int64_t theIndex, 
                const std::string& theHeaderLine = "");
        ~FileWriter();

        void write(const std::string& theContent, const time_t theTime);

        void _write(const std::string* theContent, const time_t theTime);
    private:
        void startTimer(const time_t &theTime);
        static void onTimeout(void *theArg);
        void closeFile();
        void switchFile(const time_t &theTime);


    private:
        std::string headerLineM;
        std::string modelNameM;
        unsigned indexM;
        int64_t switchTimeM;
        int64_t switchSizeM;
        std::ofstream fileStreamM;
        unsigned curWriteTimeM;
        unsigned curTimerTimeM;
        int64_t curSizeM;
        std::string tmpFilePathM;
        std::string beginTimeStrM;
        std::string dbTableNameM;
        std::string outDirM;

        CppProcessor* processorM;
        min_heap_item_t* timerHandlerM;

    };

}

#endif /* FILEWRITER_H */


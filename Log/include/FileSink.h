#ifndef FILESINK_H
#define FILESINK_H

#include "Sink.h"
#include <string>
#include <fstream>

namespace nd
{
    class FileSink: public Sink
    {
    public:
        FileSink(std::string& prefix, Severity severity);
        void setKeepNo(int keepno){ keepHisNoM = keepno; };
        void setSwitchDays(int days){ switchDaysM = days; }
        virtual ~FileSink() = default;
        virtual void log(const LogMeta* theMeta);

        void checkDelHisFile();

    private:
        std::string prefixM;
        std::string timeFormatM;
        Severity severityM;
        std::ofstream fileHandleM;

        int keepHisNoM;
        int switchDaysM;
        uint64_t curDaysM;
    };

}

#endif /* FILESINK_H */


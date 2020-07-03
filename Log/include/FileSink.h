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
        FileSink(std::string& prefix, int keepno, Severity severity);
        virtual ~FileSink() = default;
        virtual void log(const LogMeta* theMeta);

    private:
        std::string prefixM;
        int keepnoM;
        std::string timeFormatM;
        Severity severityM;
        std::ofstream fileHandleM;
    };

}

#endif /* FILESINK_H */


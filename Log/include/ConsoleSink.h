#ifndef CONSOLESINK_H
#define CONSOLESINK_H

#include "Sink.h"
#include <string>
#include <fstream>

namespace nd
{
    class ConsoleSink: public Sink
    {
    public:
        ConsoleSink(Severity severity);
        virtual ~ConsoleSink() = default;
        virtual void log(const LogMeta* theMeta);

    private:
        std::string timeFormatM;
        Severity severityM;
    };

}

#endif /* CONSOLESINK_H */


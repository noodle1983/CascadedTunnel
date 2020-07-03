#ifndef LOGSINK_H
#define LOGSINK_H

#include "LogStruct.h"

namespace nd
{
    class Sink
    {
    public:
        virtual ~Sink() = default;
        virtual void log(const LogMeta* theMeta) = 0;
    };

}

#endif /* LOGSINK_H */


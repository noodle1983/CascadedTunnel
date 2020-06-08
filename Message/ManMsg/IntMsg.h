#ifndef INTMSG_H
#define INTMSG_H

#include <stdint.h>
#include <string.h>
#include "MsgErrorCode.h"

namespace Msg
{
    class Uint8
    {
    public:
        Uint8(){}
        ~Uint8(){}

        enum {MIN_BYTES = 1};

        void init()
        {
            valueM = 0;
        }

        operator unsigned char()
        { return valueM; }
        const Uint8& operator =(const unsigned char theValue)
        { valueM = theValue; return *this;}

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(uint8_t) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            valueM = theBuffer[theIndex];
            theIndex += sizeof(uint8_t);
            return SUCCESS_E;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(uint8_t) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            theBuffer[theIndex] = valueM;
            theIndex += sizeof(uint8_t);

            return SUCCESS_E;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            theOut << unsigned(valueM);
            return theOut;
        }


    public:
        unsigned char valueM;
    };
    typedef Uint8 Length8;

    class Uint16
    {
    public:
        Uint16(){}
        ~Uint16(){}

        enum {MIN_BYTES = 2};

        void init()
        {
            valueM = 0;
        }

        operator uint16_t()
        { return valueM; }
        const Uint16& operator =(const unsigned short theValue)
        { valueM = theValue; return *this;}

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(uint16_t) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            memcpy(&valueM, theBuffer + theIndex, sizeof(uint16_t));
            theIndex += sizeof(uint16_t);
            return SUCCESS_E;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(uint16_t) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            memcpy(theBuffer + theIndex, &valueM, sizeof(uint16_t));
            theIndex += sizeof(uint16_t);

            return SUCCESS_E;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            theOut << valueM;
            return theOut;
        }

    public:
        uint16_t valueM;
    };
    /* for the length of the Message, it is reset again at last.*/
    typedef Uint16 Length16;

    class Uint32
    {
    public:
        Uint32(){}
        ~Uint32(){}

        enum {MIN_BYTES = 4};

        void init()
        {
            valueM = 0;
        }

        operator uint32_t()
        { return valueM; }
        const Uint32& operator =(const unsigned theValue)
        { valueM = theValue; return *this;}

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(uint32_t) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            memcpy(&valueM, theBuffer + theIndex, sizeof(uint32_t));
            theIndex += sizeof(uint32_t);
            return SUCCESS_E;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(uint32_t) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            memcpy(theBuffer + theIndex, &valueM, sizeof(uint32_t));
            theIndex += sizeof(uint32_t);

            return SUCCESS_E;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            theOut << valueM;
            return theOut;
        }

    public:
        uint32_t valueM;
    };
    /* for the length of the Message, it is reset again at last.*/
    typedef Uint32 Length32;
    typedef Uint32 MsgId32;

    class PlainInt32
    {
    public:
        PlainInt32(){}
        ~PlainInt32(){}

        enum {MIN_BYTES = 4};

        void init()
        {
            valueM = 0;
        }

        operator uint32_t()
        { return valueM; }
        const PlainInt32& operator =(const unsigned theValue)
        { valueM = theValue; return *this;}

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(uint32_t) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            memcpy(&valueM, theBuffer + theIndex, sizeof(uint32_t));
            theIndex += sizeof(uint32_t);
            return SUCCESS_E;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(uint32_t) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            uint32_t netInt = valueM;
            memcpy(theBuffer + theIndex, &netInt, sizeof(uint32_t));
            theIndex += sizeof(uint32_t);

            return SUCCESS_E;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            theOut << valueM;
            return theOut;
        }

    public:
        uint32_t valueM;
    };

    class Uint64
    {
    public:
        Uint64(){}
        ~Uint64(){}

        enum {MIN_BYTES = 8};

        void init()
        {
            valueM = 0;
        }

        operator uint64_t()
        { return valueM; }
        const Uint64& operator =(const unsigned long long theValue)
        { valueM = theValue; return *this;}

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(uint64_t) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            memcpy(&valueM, theBuffer + theIndex, sizeof(uint64_t));
            theIndex += sizeof(uint64_t);
            return SUCCESS_E;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(uint64_t) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            memcpy(theBuffer + theIndex, &valueM, sizeof(uint64_t));
            theIndex += sizeof(uint64_t);

            return SUCCESS_E;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            theOut << valueM;
            return theOut;
        }

    public:
        uint64_t valueM;
    };
    /* for the length of the Message, it is reset again at last.*/
    typedef Uint64 Length64;
}
#endif  /*INTMSG_H*/


// this file is generate automatically, please don't change it mannually
#ifndef MESSAGE_H
#define MESSAGE_H

#include "StrMsg.h"
#include "IntMsg.h"
#include "MsgErrorCode.h"
#include "MobileIdentity.h"
#include "TlvBcdString.h"
#include "Qos.h"
#include "Location.h"
#include "Timestamp.h"
#include "BcdString.h"
#include "Log.h"

#include <sstream>

namespace Msg
{
    enum ServiceType
    {
        SERVICE_TYPE_ACTION_ORIENTED_E = 1,
        SERVICE_TYPE_PER_SOCKET_E      = 2,
        SERVICE_TYPE_SUMMERY_E         = 3
    };

    typedef Timestamp<0x14> DateTimeIEI;

    class MsgHeader
    {
    public:
        MsgHeader(){}
        MsgHeader(int){init();}
        ~MsgHeader(){}

        enum{ ID = 0x00};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();            
            messageType.init();            
            proxyFd.init();            
        } /* end of void init(...) */

        int decodeHeader(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse MsgHeader.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + MsgHeader::MIN_BYTES;
            if (theLen < endIndex)
            {
                LOG_ERROR("MsgHeader.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse MsgHeader.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse MsgHeader.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse MsgHeader.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("MsgHeader.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse MsgHeader.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse MsgHeader.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode MsgHeader.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            messageType.valueM = MsgHeader::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode MsgHeader.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode MsgHeader.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_DEBUG("failed to encode MsgHeader.length");
                return ret; 
            }

            return ret;
        } /* end of int encode(...) */

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            if (0 == theLayer)
            {
                theOut << "\n" <<leadStr << "MsgHeader";
            }
            else
            {
                theOut << "MsgHeader";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "proxyFd: ";
            proxyFd.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;           
        MsgId32 messageType;           
        Uint32 proxyFd;           
    }; /* end of class MsgHeader */


    class HeartbeatReq
    {
    public:
        HeartbeatReq(){}
        HeartbeatReq(int){init();}
        ~HeartbeatReq(){}

        enum{ ID = 0x01};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();            
            messageType.init();            
            proxyFd.init();            
        } /* end of void init(...) */

        int decodeHeader(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse HeartbeatReq.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + HeartbeatReq::MIN_BYTES;
            if (theLen < endIndex)
            {
                LOG_ERROR("HeartbeatReq.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse HeartbeatReq.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse HeartbeatReq.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse HeartbeatReq.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("HeartbeatReq.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse HeartbeatReq.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse HeartbeatReq.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode HeartbeatReq.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            messageType.valueM = HeartbeatReq::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode HeartbeatReq.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode HeartbeatReq.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_DEBUG("failed to encode HeartbeatReq.length");
                return ret; 
            }

            return ret;
        } /* end of int encode(...) */

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            if (0 == theLayer)
            {
                theOut << "\n" <<leadStr << "HeartbeatReq";
            }
            else
            {
                theOut << "HeartbeatReq";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "proxyFd: ";
            proxyFd.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;           
        MsgId32 messageType;           
        Uint32 proxyFd;           
    }; /* end of class HeartbeatReq */


    class HeartbeatRsp
    {
    public:
        HeartbeatRsp(){}
        HeartbeatRsp(int){init();}
        ~HeartbeatRsp(){}

        enum{ ID = 0x02};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();            
            messageType.init();            
            proxyFd.init();            
        } /* end of void init(...) */

        int decodeHeader(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse HeartbeatRsp.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + HeartbeatRsp::MIN_BYTES;
            if (theLen < endIndex)
            {
                LOG_ERROR("HeartbeatRsp.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse HeartbeatRsp.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse HeartbeatRsp.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse HeartbeatRsp.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("HeartbeatRsp.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse HeartbeatRsp.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse HeartbeatRsp.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode HeartbeatRsp.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            messageType.valueM = HeartbeatRsp::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode HeartbeatRsp.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode HeartbeatRsp.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_DEBUG("failed to encode HeartbeatRsp.length");
                return ret; 
            }

            return ret;
        } /* end of int encode(...) */

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            if (0 == theLayer)
            {
                theOut << "\n" <<leadStr << "HeartbeatRsp";
            }
            else
            {
                theOut << "HeartbeatRsp";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "proxyFd: ";
            proxyFd.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;           
        MsgId32 messageType;           
        Uint32 proxyFd;           
    }; /* end of class HeartbeatRsp */


    class NewConnection
    {
    public:
        NewConnection(){}
        NewConnection(int){init();}
        ~NewConnection(){}

        enum{ ID = 0x03};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();            
            messageType.init();            
            proxyFd.init();            
            winOffset.init();            
        } /* end of void init(...) */

        int decodeHeader(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse NewConnection.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + NewConnection::MIN_BYTES;
            if (theLen < endIndex)
            {
                LOG_ERROR("NewConnection.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse NewConnection.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse NewConnection.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = winOffset.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse NewConnection.winOffset");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse NewConnection.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("NewConnection.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse NewConnection.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse NewConnection.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = winOffset.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse NewConnection.winOffset");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode NewConnection.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            messageType.valueM = NewConnection::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode NewConnection.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode NewConnection.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = winOffset.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode NewConnection.winOffset");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_DEBUG("failed to encode NewConnection.length");
                return ret; 
            }

            return ret;
        } /* end of int encode(...) */

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            if (0 == theLayer)
            {
                theOut << "\n" <<leadStr << "NewConnection";
            }
            else
            {
                theOut << "NewConnection";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "proxyFd: ";
            proxyFd.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "winOffset: ";
            winOffset.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;           
        MsgId32 messageType;           
        Uint32 proxyFd;           
        Uint32 winOffset;           
    }; /* end of class NewConnection */


    class ConnectionClosed
    {
    public:
        ConnectionClosed(){}
        ConnectionClosed(int){init();}
        ~ConnectionClosed(){}

        enum{ ID = 0x04};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();            
            messageType.init();            
            proxyFd.init();            
        } /* end of void init(...) */

        int decodeHeader(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ConnectionClosed.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + ConnectionClosed::MIN_BYTES;
            if (theLen < endIndex)
            {
                LOG_ERROR("ConnectionClosed.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ConnectionClosed.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ConnectionClosed.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ConnectionClosed.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("ConnectionClosed.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ConnectionClosed.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ConnectionClosed.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode ConnectionClosed.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            messageType.valueM = ConnectionClosed::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode ConnectionClosed.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode ConnectionClosed.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_DEBUG("failed to encode ConnectionClosed.length");
                return ret; 
            }

            return ret;
        } /* end of int encode(...) */

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            if (0 == theLayer)
            {
                theOut << "\n" <<leadStr << "ConnectionClosed";
            }
            else
            {
                theOut << "ConnectionClosed";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "proxyFd: ";
            proxyFd.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;           
        MsgId32 messageType;           
        Uint32 proxyFd;           
    }; /* end of class ConnectionClosed */


    class RProxyConClose
    {
    public:
        RProxyConClose(){}
        RProxyConClose(int){init();}
        ~RProxyConClose(){}

        enum{ ID = 0x05};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();            
            messageType.init();            
            proxyFd.init();            
        } /* end of void init(...) */

        int decodeHeader(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxyConClose.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + RProxyConClose::MIN_BYTES;
            if (theLen < endIndex)
            {
                LOG_ERROR("RProxyConClose.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxyConClose.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxyConClose.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxyConClose.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("RProxyConClose.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxyConClose.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxyConClose.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode RProxyConClose.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            messageType.valueM = RProxyConClose::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode RProxyConClose.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode RProxyConClose.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_DEBUG("failed to encode RProxyConClose.length");
                return ret; 
            }

            return ret;
        } /* end of int encode(...) */

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            if (0 == theLayer)
            {
                theOut << "\n" <<leadStr << "RProxyConClose";
            }
            else
            {
                theOut << "RProxyConClose";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "proxyFd: ";
            proxyFd.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;           
        MsgId32 messageType;           
        Uint32 proxyFd;           
    }; /* end of class RProxyConClose */


    class RProxySlowDown
    {
    public:
        RProxySlowDown(){}
        RProxySlowDown(int){init();}
        ~RProxySlowDown(){}

        enum{ ID = 0x06};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();            
            messageType.init();            
            proxyFd.init();            
        } /* end of void init(...) */

        int decodeHeader(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxySlowDown.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + RProxySlowDown::MIN_BYTES;
            if (theLen < endIndex)
            {
                LOG_ERROR("RProxySlowDown.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxySlowDown.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxySlowDown.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxySlowDown.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("RProxySlowDown.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxySlowDown.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxySlowDown.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode RProxySlowDown.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            messageType.valueM = RProxySlowDown::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode RProxySlowDown.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode RProxySlowDown.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_DEBUG("failed to encode RProxySlowDown.length");
                return ret; 
            }

            return ret;
        } /* end of int encode(...) */

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            if (0 == theLayer)
            {
                theOut << "\n" <<leadStr << "RProxySlowDown";
            }
            else
            {
                theOut << "RProxySlowDown";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "proxyFd: ";
            proxyFd.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;           
        MsgId32 messageType;           
        Uint32 proxyFd;           
    }; /* end of class RProxySlowDown */


    class RProxySpeedUp
    {
    public:
        RProxySpeedUp(){}
        RProxySpeedUp(int){init();}
        ~RProxySpeedUp(){}

        enum{ ID = 0x07};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();            
            messageType.init();            
            proxyFd.init();            
        } /* end of void init(...) */

        int decodeHeader(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxySpeedUp.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + RProxySpeedUp::MIN_BYTES;
            if (theLen < endIndex)
            {
                LOG_ERROR("RProxySpeedUp.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxySpeedUp.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxySpeedUp.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxySpeedUp.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("RProxySpeedUp.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxySpeedUp.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse RProxySpeedUp.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode RProxySpeedUp.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            messageType.valueM = RProxySpeedUp::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode RProxySpeedUp.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode RProxySpeedUp.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_DEBUG("failed to encode RProxySpeedUp.length");
                return ret; 
            }

            return ret;
        } /* end of int encode(...) */

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            if (0 == theLayer)
            {
                theOut << "\n" <<leadStr << "RProxySpeedUp";
            }
            else
            {
                theOut << "RProxySpeedUp";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "proxyFd: ";
            proxyFd.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;           
        MsgId32 messageType;           
        Uint32 proxyFd;           
    }; /* end of class RProxySpeedUp */


    class ProxyReq
    {
    public:
        ProxyReq(){}
        ProxyReq(int){init();}
        ~ProxyReq(){}

        enum{ ID = 0x08};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        PlainString::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();            
            messageType.init();            
            proxyFd.init();            
            payload.init();            
        } /* end of void init(...) */

        int decodeHeader(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyReq.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + ProxyReq::MIN_BYTES;
            if (theLen < endIndex)
            {
                LOG_ERROR("ProxyReq.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyReq.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyReq.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = payload.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyReq.payload");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyReq.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("ProxyReq.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyReq.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyReq.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = payload.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyReq.payload");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode ProxyReq.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            messageType.valueM = ProxyReq::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode ProxyReq.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode ProxyReq.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = payload.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode ProxyReq.payload");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_DEBUG("failed to encode ProxyReq.length");
                return ret; 
            }

            return ret;
        } /* end of int encode(...) */

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            if (0 == theLayer)
            {
                theOut << "\n" <<leadStr << "ProxyReq";
            }
            else
            {
                theOut << "ProxyReq";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "proxyFd: ";
            proxyFd.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "payload: ";
            payload.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;           
        MsgId32 messageType;           
        Uint32 proxyFd;           
        PlainString payload;           
    }; /* end of class ProxyReq */


    class ProxyRsp
    {
    public:
        ProxyRsp(){}
        ProxyRsp(int){init();}
        ~ProxyRsp(){}

        enum{ ID = 0x09};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        PlainString::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();            
            messageType.init();            
            proxyFd.init();            
            payload.init();            
        } /* end of void init(...) */

        int decodeHeader(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyRsp.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + ProxyRsp::MIN_BYTES;
            if (theLen < endIndex)
            {
                LOG_ERROR("ProxyRsp.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyRsp.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyRsp.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = payload.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyRsp.payload");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyRsp.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("ProxyRsp.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyRsp.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyRsp.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = payload.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse ProxyRsp.payload");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode ProxyRsp.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            messageType.valueM = ProxyRsp::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode ProxyRsp.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode ProxyRsp.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = payload.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode ProxyRsp.payload");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_DEBUG("failed to encode ProxyRsp.length");
                return ret; 
            }

            return ret;
        } /* end of int encode(...) */

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            if (0 == theLayer)
            {
                theOut << "\n" <<leadStr << "ProxyRsp";
            }
            else
            {
                theOut << "ProxyRsp";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "proxyFd: ";
            proxyFd.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "payload: ";
            payload.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;           
        MsgId32 messageType;           
        Uint32 proxyFd;           
        PlainString payload;           
    }; /* end of class ProxyRsp */


    class SyncWinSize
    {
    public:
        SyncWinSize(){}
        SyncWinSize(int){init();}
        ~SyncWinSize(){}

        enum{ ID = 0x0a};

        enum
        {
            MIN_BYTES =
                        Length32::MIN_BYTES +
                        MsgId32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        0
        }; /* end of enum MIN_BYTES */

        void init()
        {
            length.init();            
            messageType.init();            
            proxyFd.init();            
            winOffset.init();            
        } /* end of void init(...) */

        int decodeHeader(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse SyncWinSize.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + SyncWinSize::MIN_BYTES;
            if (theLen < endIndex)
            {
                LOG_ERROR("SyncWinSize.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse SyncWinSize.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse SyncWinSize.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = winOffset.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse SyncWinSize.winOffset");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse SyncWinSize.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            unsigned endIndex = theIndex - Length32::MIN_BYTES + length.valueM;
            if (theLen < endIndex)
            {
                LOG_ERROR("SyncWinSize.length not enough buffer");
                return NOT_ENOUGH_BUFFER_E;
            }
            ret = messageType.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse SyncWinSize.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse SyncWinSize.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = winOffset.decode(theBuffer, endIndex, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to parse SyncWinSize.winOffset");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            return ret;
        } /* end of int decode(...) */

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            unsigned startIndex = theIndex;
            ret = length.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode SyncWinSize.length");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            messageType.valueM = SyncWinSize::ID;
            ret = messageType.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode SyncWinSize.messageType");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = proxyFd.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode SyncWinSize.proxyFd");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }

            ret = winOffset.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_ERROR("failed to encode SyncWinSize.winOffset");
                std::ostringstream oss;    
                dump(oss);
                LOG_DEBUG(oss.str());
                return ret;
            }


            //re-encode the length
            length.valueM = theIndex - startIndex;
            ret = length.encode(theBuffer, theLen, startIndex);
            if (SUCCESS_E != ret)            
            {
                LOG_DEBUG("failed to encode SyncWinSize.length");
                return ret; 
            }

            return ret;
        } /* end of int encode(...) */

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            if (0 == theLayer)
            {
                theOut << "\n" <<leadStr << "SyncWinSize";
            }
            else
            {
                theOut << "SyncWinSize";
            }
            leadStr.append("    ");

            theOut << "\n" << leadStr << "length: ";
            length.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "messageType: ";
            messageType.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "proxyFd: ";
            proxyFd.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "winOffset: ";
            winOffset.dump(theOut, theLayer + 1);

            if (0 == theLayer)
            {
                theOut << "\n";
            }
            return theOut;
        } /* end of dump(...) */


    public:
        Length32 length;           
        MsgId32 messageType;           
        Uint32 proxyFd;           
        Uint32 winOffset;           
    }; /* end of class SyncWinSize */

}

#endif /* MESSAGE_H */


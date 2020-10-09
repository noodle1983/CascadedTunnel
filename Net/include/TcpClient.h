#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "SocketConnection.h"

#include <string>
#include <mutex>
#include <memory>
#include <event.h>


namespace nd
{
    class IClientProtocol;
    class TcpClient;

    typedef std::shared_ptr<TcpClient> TcpClientPtr;
    typedef std::weak_ptr<TcpClient> TcpClientWPtr;
    class TcpClient
    {
    public:
        TcpClient(
            IClientProtocol* theProtocol,
            Reactor* theReactor,
            CppProcessor* theProcessor);
        ~TcpClient();
        void deleteSelf();
        TcpClientPtr self(){return selfM;}
        SocketConnectionPtr getConnection(){return connectionM;}
        IClientProtocol* getProtocol(){return protocolM;}

        /**
         * connect in a async way.
         * return:
         *         -1 : there is an error.
         *         0  : connected or blocked
         */
        int connect();
        int close();
        bool isClose()
        {
            auto connection = connectionM;
            if (connection.get())
            {
                return connection->isClose();
            }
            return true;
        }
        bool isConnected()
        {
            auto connection = connectionM;
            return connection.get() && isConnectedM;
        }
        unsigned sendn(const char* theBuffer, const unsigned theLen);
        template<typename Msg>
        unsigned sendMsg(Msg& msg);


        /**
         * onXxx
         * Description: TcpClient will be notified if it is xxx.
         * And do not rmClient in these functions
         */
        void onClientTimeout();
        void onConnected(int theFd, SocketConnectionPtr theConnection);
        void onError(SocketConnectionPtr theConnection);

        void resetTimer(){reconnectTimerEvtM = NULL;}

    private:
        void _close();
        void _connect();
        void _deleteSelf();
        void _reconnectLater();

        IClientProtocol* protocolM;
        Reactor* reactorM;
        CppProcessor* processorM;

        std::string peerAddrM;
        int peerPortM;

        volatile size_t isClosedM;
        volatile size_t isConnectedM;

        SocketConnectionPtr connectionM;
        TcpClientPtr selfM;
        min_heap_item_t* reconnectTimerEvtM;

        size_t connectTimesM;
        uint64_t processorIdM;
    };

    inline unsigned
    TcpClient::sendn(const char* theBuffer, const unsigned theLen)
    {
        if (isClosedM) {return 0;}
        auto connection = connectionM;
        if (connection.get() && (isConnectedM || connectTimesM == 1))
        {
            return connection->sendn(theBuffer, theLen);
        }
        else
        {
            return 0;
        }

    }
    template<typename Msg>
    unsigned TcpClient::sendMsg(Msg& msg)
    {
        if (isClosedM) {return 0;}
        auto connection = connectionM;
        if (connection.get() && (isConnectedM || connectTimesM == 1))
        {
            return connection->sendMsg(msg);
        }
        else
        {
            return 0;
        }
    }
}

#endif /* TCPCLIENT_H */


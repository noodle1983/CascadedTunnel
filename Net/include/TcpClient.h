#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "SocketConnection.h"

#include <string>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <event.h>


namespace Net
{
class IClientProtocol;
namespace Client
{
    class TcpClient;
    typedef boost::shared_ptr<TcpClient> TcpClientPtr;
    typedef boost::weak_ptr<TcpClient> TcpClientWPtr;
    class TcpClient
    {
    public:
        TcpClient(
            IClientProtocol* theProtocol,
            Reactor::Reactor* theReactor,
            Processor::BoostProcessor* theProcessor);
        ~TcpClient();
        void deleteSelf();
        TcpClientPtr self(){return selfM;}
        Connection::SocketConnectionPtr getConnection(){return connectionM;}

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
            boost::lock_guard<boost::mutex> lock(connectionMutexM);
            if (connectionM.get())
            {
                return connectionM->isClose();
            }
            return true;
        }
        bool isConnected()
        {
            boost::lock_guard<boost::mutex> lock(connectionMutexM);
            return connectionM.get() && isConnectedM;
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
        void onConnected(int theFd, Connection::SocketConnectionPtr theConnection);
        void onError(Connection::SocketConnectionPtr theConnection);

        void resetTimer(){reconnectTimerEvtM = NULL;}

    private:
        void _deleteSelf();
        void reconnectLater();

        IClientProtocol* protocolM;
        Reactor::Reactor* reactorM;
        Processor::BoostProcessor* processorM;

        std::string peerAddrM;
        int peerPortM;

        mutable bool isClosedM;

        boost::mutex connectionMutexM;
        bool isConnectedM;
        Net::Connection::SocketConnectionPtr connectionM;
        TcpClientPtr selfM;
        min_heap_item_t* reconnectTimerEvtM;

        size_t connectTimesM;
    };

    inline unsigned
    TcpClient::sendn(const char* theBuffer, const unsigned theLen)
    {
        if (isClosedM) {return 0;}
        boost::lock_guard<boost::mutex> lock(connectionMutexM);
        if (connectionM.get() && (isConnectedM || connectTimesM == 1))
        {
            return connectionM->sendn(theBuffer, theLen);
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
        boost::lock_guard<boost::mutex> lock(connectionMutexM);
        if (connectionM.get() && (isConnectedM || connectTimesM == 1))
        {
            return connectionM->sendMsg(msg);
        }
        else
        {
            return 0;
        }
    }
}
}

#endif /* TCPCLIENT_H */


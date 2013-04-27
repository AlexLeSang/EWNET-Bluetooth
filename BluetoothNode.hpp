#ifndef BLUETOOTHNODE_HPP
#define BLUETOOTHNODE_HPP

#include <QList>

#include "Message.hpp"
#include "CoreFacade.hpp"

/*!
 * \brief MAX_MESSAGE_NUMBER
 */
constexpr quint32 MAX_MESSAGE_NUMBER = 100;

class Environment;

/*!
 * \brief The BluetoothNode class
 */
class BluetoothNode {
public:
    void send( Environment & environmentRef );
    void receive( Environment & environmentRef );
    QList < Message > & getReceiveMessages();
    virtual void reinit();
    void sendReceivedPacketsNumber();
    void sendTransmitedPackageNumber();
    quint32 getTransmitedPackageCounter() const;
    quint32 getReceivePackageCounter() const;

protected:
    void addMessageToSend( const Message & addMessage );
    void prepareNextMessage();
    Message & getMessageToSend();
    void addMessageToReceived( const Message & addMessage );

protected:
    void ( CoreFacade::* addToLog )( QString );
    QString nameString;
    quint32 receivePackageCounter = 0;
    quint32 transmitedPackageCounter = 1;
    QList< Message > sendMessageList;
    QList< Message > receiveMessageList;
};

#endif // BLUETOOTHNODE_HPP

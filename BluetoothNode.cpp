#include "BluetoothNode.hpp"

#include "Environment.hpp"
#include "Message.hpp"

/*!
 * \brief BluetoothNode::send
 * \param environmentRef
 */
void BluetoothNode::send( Environment &environmentRef )  {
    auto message = getMessageToSend();

    QString logMessage( '\n' + nameString + ": send information : " + QString( message.getInformationFromPayload() ) );
    ( CoreFacade::getInstance().*addToLog )( logMessage );

    message.packMessage();
    environmentRef.push( message );

    ++ transmitedPackageCounter;
}

/*!
 * \brief BluetoothNode::receive
 * \param environmentRef
 */
void BluetoothNode::receive( Environment &environmentRef ) {
    QString logMessage( "" );

    /*!
     * \brief message
     */
    auto message = environmentRef.pop();
    message.unpackMessage();

    /*!
     * \brief resultValid
     */
    auto resultValid = message.isValid();

    if ( ALL_OK == resultValid ) {
        logMessage += QString( '\n' + nameString + ": Message received" );
        logMessage += QString( "\nInformation : " + QString( message.getInformationFromPayload() ) );
        ++ receivePackageCounter;

        if ( message.getARQN() ) {
            logMessage += QString( "\nArqn received, resend previous message" );
        }
        else {
            prepareNextMessage(); // Prepare next message for sending
        }
        getMessageToSend().setARQN( false );
        addMessageToReceived( message );
    }
    else {
        logMessage += QString( nameString + ": Received corrupter message! Ask for resending" );
        getMessageToSend().setARQN( true ); // Ask for resend

        if ( HEADER_ERROR == resultValid ) {
            logMessage += QString( "\nError in header detected" );
            resultValid -= HEADER_ERROR;
        }
        if ( PAYLOAD_ERROR == resultValid ) {
            logMessage += QString( "\nError in payload detected" );
        }
    }

    ( CoreFacade::getInstance().*addToLog )( logMessage );
}

/*!
 * \brief BluetoothNode::getReceiveMessages
 * \return
 */
QList<Message> &BluetoothNode::getReceiveMessages()  {
    return receiveMessageList;
}

/*!
 * \brief BluetoothNode::reinit
 */
void BluetoothNode::reinit() {
    receivePackageCounter = 0;
    transmitedPackageCounter = 0;
}

/*!
 * \brief BluetoothNode::sendReceivedPacketsNumber
 */
void BluetoothNode::sendReceivedPacketsNumber() {
    const QString logMessage( nameString + " receive " + QString::number( receivePackageCounter ) );
    ( CoreFacade::getInstance().*addToLog )( logMessage );
}

/*!
 * \brief BluetoothNode::sendTransmitedPackageNumber
 */
void BluetoothNode::sendTransmitedPackageNumber() {
    const QString logMessage( nameString + " tramsite " + QString::number( transmitedPackageCounter ) );
    ( CoreFacade::getInstance().*addToLog )( logMessage );
}

/*!
 * \brief BluetoothNode::getTransmitedPackageCounter
 * \return
 */
quint32 BluetoothNode::getTransmitedPackageCounter() const {
    return transmitedPackageCounter;
}

quint32 BluetoothNode::getReceivePackageCounter() const {
    return receivePackageCounter;
}

/*!
 * \brief BluetoothNode::addMessageToSend
 * \param addMessage
 */
void BluetoothNode::addMessageToSend( const Message &addMessage ) {
    if ( quint32( sendMessageList.size() ) < MAX_MESSAGE_NUMBER ) {
        sendMessageList.append( addMessage );
    }
}

/*!
 * \brief BluetoothNode::prepareNextMessage
 */
void BluetoothNode::prepareNextMessage()  {
    if ( ! sendMessageList.isEmpty() ) {
        sendMessageList.takeLast();
    }
}

/*!
 * \brief BluetoothNode::getMessageToSend
 * \return
 */
Message &BluetoothNode::getMessageToSend() {
    return sendMessageList.last();
}

/*!
 * \brief BluetoothNode::addMessageToReceived
 * \param addMessage
 */
void BluetoothNode::addMessageToReceived(const Message &addMessage) {
    if ( quint32( receiveMessageList.size() ) < MAX_MESSAGE_NUMBER ) {
        receiveMessageList.append( addMessage );
    }
}

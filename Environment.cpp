#include "Environment.hpp"

#include <QDebug>

#include "CoreFacade.hpp"

/*!
 * \brief Environment::channelErrorProbability
 */
qreal Environment::channelErrorProbability = 0.5;

qreal Environment::channelErrorProbabilityFrom = 0.1;
qreal Environment::channelErrorProbabilityTo = 0.99;
qreal Environment::channelErrorProbabilityStep = 0.1;

/*!
 * \brief Environment::getInstance
 * \return
 */
Environment &Environment::getInstance() {
    static Environment instance;
    return instance;
}

/*!
 * \brief Environment::~Environment
 */
Environment::~Environment() { /*Empty*/ }

/*!
 * \brief Environment::push
 * \param message
 */
void Environment::push( const Message &message ) {
    transmitedMessage = message;
}

/*!
 * \brief Environment::transmit
 */
void Environment::transmit() {
    if ( Rand::randQReal() <= channelErrorProbability ) {
        auto corruptedBit = Environment::corrupt( transmitedMessage );

        QString message( "Corrupt bit #" + QString::number( corruptedBit ) );
        CoreFacade::getInstance().sendToEnvironmentLog( message );
    }

}

/*!
 * \brief Environment::pop
 * \return
 */
Message Environment::pop() {
    Message returnMessage( transmitedMessage );
    transmitedMessage.clear();
    return returnMessage;
}

/*!
 * \brief Environment::setErrorProbability
 * \param probability
 */
void Environment::setErrorProbability( qreal probability ) {
    channelErrorProbability = probability;
}

/*!
 * \brief Environment::setErrorProbabilityFrom
 * \param probability
 */
void Environment::setErrorProbabilityFrom(qreal probability) {
    channelErrorProbabilityFrom = probability;
}

/*!
 * \brief Environment::setErrorProbabilityTo
 * \param probability
 */
void Environment::setErrorProbabilityTo(qreal probability) {
    channelErrorProbabilityTo = probability;
}

/*!
 * \brief Environment::setErrorProbabilityStep
 * \param probability
 */
void Environment::setErrorProbabilityStep(qreal probability) {
    channelErrorProbabilityStep = probability;
}

/*!
 * \brief Environment::getErrorProbability
 * \return
 */
qreal Environment::getErrorProbability() {
    return channelErrorProbability;
}

/*!
 * \brief Environment::getErrorProbabilityFrom
 * \return
 */
qreal Environment::getErrorProbabilityFrom() {
    return channelErrorProbabilityFrom;
}

/*!
 * \brief Environment::getErrorProbabilityTo
 * \return
 */
qreal Environment::getErrorProbabilityTo() {
    return channelErrorProbabilityTo;
}

/*!
 * \brief Environment::getErrorProbabilityStep
 * \return
 */
qreal Environment::getErrorProbabilityStep() {
    return channelErrorProbabilityStep;
}

/*!
 * \brief Environment::reinit
 */
void Environment::reinit() {
    transmitedMessage = Message();
}

/*!
 * \brief Environment::Environment
 */
Environment::Environment() : transmitedMessage() { /*Empty*/ }

/*!
 * \brief Environment::corrupt
 * \param message
 * \return
 */
quint32 Environment::corrupt( Message &message )  {

    /*!
     * \brief accessCodeRef
     */
    auto & accessCodeRef( message.packet().accessCodeData.data() );

    /*!
     * \brief headerRef
     */
    auto & headerRef( message.packet().headerData.data() );

    /*!
     * \brief payloadRef
     */
    auto & payloadRef( message.packet().payloadData.data() );

    /*!
     * \brief accesCodeSize
     */
    auto accesCodeSize = quint32( accessCodeRef.size() );

    /*!
     * \brief headerSize
     */
    auto headerSize = quint32( headerRef.size() );

    /*!
     * \brief payloadSize
     */
    auto payloadSize = quint32( payloadRef.size() );


    /*!
     * \brief randBit
     */
    quint32 randBit = Rand::randInRange( accesCodeSize + headerSize + payloadSize );

    // randBit in access code
    if ( randBit < accesCodeSize ) {
        accessCodeRef.setBit( randBit, ! accessCodeRef.at( randBit ) );
        return randBit;
    }
    else {
        randBit -= accesCodeSize;
    }

    // randBit in header
    if ( randBit < headerSize ) {
        headerRef.setBit( randBit, ! headerRef.at( randBit ) );
    }
    else {
        randBit -= headerSize;
    }

    // randBit in payload
    if ( randBit < payloadSize ) {
        payloadRef.setBit( randBit, ! payloadRef.at( randBit ) );
    }

    return randBit;
}

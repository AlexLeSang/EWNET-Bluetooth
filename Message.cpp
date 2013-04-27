#include "Message.hpp"

#include <QDebug>

/*!
 * \brief Message::Message
 */
Message::Message() :
    QObject( nullptr ),

    headerFEC( false ),
    headerChecksum( false ),
    headerValid( true ),

    payloadChecksum( false ),
    payloadValid( true ),

    messagePacket( ACCESS_CODE_LENGHT, HEADER_LENGHT, PAYLOAD_LENGHT - CRC_16_LENGHT )
{ /*Empty*/ }

/*!
 * \brief Message::Message
 * \param message
 */
Message::Message( const Message & message ) :
    QObject( nullptr ),


    messagePacket( message.constPacket().accessCodeData,
                   message.constPacket().headerData,
                   message.constPacket().payloadData )
{
    headerFEC = message.headerHasFEC();
    headerChecksum = message.headerHasChecksum();
    headerValid = message.isHeaderValid();

    payloadChecksum = message.payloadHasChecksum();
    payloadValid = message.isPayloadValid();
}


/*!
 * \brief Message::operator =
 * \param rMessage
 * \return
 */
Message &Message::operator =( const Message & rMessage ) {
    messagePacket.accessCodeData = rMessage.constPacket().accessCodeData;
    messagePacket.headerData = rMessage.constPacket().headerData;
    messagePacket.payloadData = rMessage.constPacket().payloadData;

    headerFEC = rMessage.headerHasFEC();
    headerChecksum = rMessage.headerHasChecksum();
    headerValid = rMessage.isHeaderValid();

    payloadChecksum = rMessage.payloadHasChecksum();
    payloadValid = rMessage.isPayloadValid();
    return *this;
}

/*!
 * \brief Message::clear
 */
void Message::clear() {
    messagePacket.accessCodeData.clear();
    messagePacket.headerData.clear();
    messagePacket.payloadData.clear();
}

/*!
 * \brief Message::constPacket
 * \return
 */
const Message::Packet &Message::constPacket() const {
    return messagePacket;
}

/*!
 * \brief Message::packMessage
 */
void Message::packMessage() {
    packHeader();
    packPayload();
}


/*!
 * \brief Message::unpackMessage
 */
void Message::unpackMessage() {
    unpackHeader();
    unpackPayload();
}


/*!
 * \brief Message::isValid
 * \return
 */
int Message::isValid() const {
    int valid = ALL_OK;
    if( !isHeaderValid() ) {
        valid += HEADER_ERROR;
    }
    if ( !isPayloadValid() ) {
        valid += PAYLOAD_ERROR;
    }
    return valid;
}

/*!
 * \brief Message::packet
 * \return
 */
Message::Packet &Message::packet() {
    return messagePacket;
}

/*!
 * \brief Message::headerHasFEC
 * \return
 */
bool Message::headerHasFEC() const {
    return headerFEC;
}

/*!
 * \brief Message::headerHasChecksum
 * \return
 */
bool Message::headerHasChecksum() const {
    return headerChecksum;
}

/*!
 * \brief Message::setAM_ADDR
 * \param am_addr
 */
void Message::setAM_ADDR( quint8 am_addr ) {
    removeHeaderFEC();
    removeHeaderChecksum();

    QBitArray bitAM_ADDR = QBitArrayHelper::uintToQBitArray< AM_ADDR_LENGHT >( am_addr );

    for ( qint32 headerIndex = messagePacket.headerData.size() - 1, am_addIndex = bitAM_ADDR.size() - 1; am_addIndex != -1; -- headerIndex, -- am_addIndex ) {
        messagePacket.headerData.set( headerIndex, bitAM_ADDR.at( am_addIndex ) );
    }

    addHeaderChecksum();
    addHeaderFEC();
}

/*!
 * \brief Message::setTYPE
 * \param type
 */
void Message::setTYPE( quint8 type ) {
    QBitArray bitType = QBitArrayHelper::uintToQBitArray< TYPE_LENGHT >( type );

    removeHeaderFEC();
    removeHeaderChecksum();

    for ( qint32 headerIndex = messagePacket.headerData.size() - 1 - AM_ADDR_LENGHT,
          typeIndex = bitType.size() - 1;
          typeIndex != -1; -- headerIndex, -- typeIndex ) {
        messagePacket.headerData.set( headerIndex, bitType.at( typeIndex ) );
    }

    addHeaderChecksum();
    addHeaderFEC();
}

/*!
 * \brief Message::setFLOW
 * \param flow
 */
void Message::setFLOW( bool flow ) {
    removeHeaderFEC();
    removeHeaderChecksum();

    messagePacket.headerData.set( messagePacket.headerData.size() - 1 - AM_ADDR_LENGHT - TYPE_LENGHT, flow );

    addHeaderChecksum();
    addHeaderFEC();
}

/*!
 * \brief Message::setARQN
 * \param arqn
 */
void Message::setARQN( bool arqn ) {
    removeHeaderFEC();
    removeHeaderChecksum();

    messagePacket.headerData.set( messagePacket.headerData.size() - 1 - AM_ADDR_LENGHT - TYPE_LENGHT - FLOW_LENGHT, arqn );

    addHeaderChecksum();
    addHeaderFEC();
}

/*!
 * \brief Message::setSEQN
 * \param seqn
 */
void Message::setSEQN( bool seqn ) {
    removeHeaderFEC();
    removeHeaderChecksum();

    messagePacket.headerData.set( messagePacket.headerData.size() - 1 - AM_ADDR_LENGHT - TYPE_LENGHT - FLOW_LENGHT - ARQN_LENGTH, seqn );

    addHeaderChecksum();
    addHeaderFEC();
}

/*!
 * \brief Message::getAM_ADDR
 * \return
 */
quint8 Message::getAM_ADDR() {
    removeHeaderFEC();
    removeHeaderChecksum();

    QBitArray bitAM_ADDR( AM_ADDR_LENGHT );

    for ( qint32 headerIndex = messagePacket.headerData.size() - 1, am_addIndex = bitAM_ADDR.size() - 1; am_addIndex != -1; -- headerIndex, -- am_addIndex ) {
        bitAM_ADDR.setBit( am_addIndex, messagePacket.headerData.at( headerIndex ) );
    }

    addHeaderChecksum();
    addHeaderFEC();

    return QBitArrayHelper::qBitArrayToUint( bitAM_ADDR );
}

/*!
 * \brief Message::getTYPE
 * \return
 */
quint8 Message::getTYPE() {
    removeHeaderFEC();
    removeHeaderChecksum();

    QBitArray typeAM_ADDR( TYPE_LENGHT );

    for ( qint32 headerIndex = messagePacket.headerData.size() - 1 - AM_ADDR_LENGHT,
          typeIndex = typeAM_ADDR.size() - 1;
          typeIndex != -1; -- headerIndex, -- typeIndex ) {
        typeAM_ADDR.setBit( typeIndex, messagePacket.headerData.at( headerIndex ) );
    }

    addHeaderChecksum();
    addHeaderFEC();

    return QBitArrayHelper::qBitArrayToUint( typeAM_ADDR );
}

/*!
 * \brief Message::getFLOW
 * \return
 */
bool Message::getFLOW() {
    removeHeaderFEC();
    removeHeaderChecksum();

    auto flow = messagePacket.headerData.at( messagePacket.headerData.size() - 1 - AM_ADDR_LENGHT - TYPE_LENGHT );

    addHeaderChecksum();
    addHeaderFEC();

    return flow;
}

/*!
 * \brief Message::getARQN
 * \return
 */
bool Message::getARQN() {
    removeHeaderFEC();
    removeHeaderChecksum();

    auto arqn = messagePacket.headerData.at( messagePacket.headerData.size() - 1 - AM_ADDR_LENGHT - TYPE_LENGHT - FLOW_LENGHT );

    addHeaderChecksum();
    addHeaderFEC();

    return arqn;
}

/*!
 * \brief Message::getSEQN
 * \return
 */
bool Message::getSEQN() {
    removeHeaderFEC();
    removeHeaderChecksum();

    auto seqn = messagePacket.headerData.at( messagePacket.headerData.size() - 1 - AM_ADDR_LENGHT - TYPE_LENGHT - FLOW_LENGHT - ARQN_LENGTH );

    addHeaderChecksum();
    addHeaderFEC();

    return seqn;
}

/*!
 * \brief Message::addPayloadChecksum
 */
void Message::addPayloadChecksum() {
    if ( !payloadHasChecksum() ) {

        DataArray payloadWithChecksum( CRC_16::addCRC( messagePacket.payloadData.constData() ) );

        messagePacket.payloadData = payloadWithChecksum;
        payloadChecksum = true;
    }
}

/*!
 * \brief Message::removePayloadChecksum
 */
void Message::removePayloadChecksum() {
    if ( payloadHasChecksum() ) {

        DataArray payloadWithoutChecksum( CRC_16::removeCRC( messagePacket.payloadData.constData() ) );

        messagePacket.payloadData = payloadWithoutChecksum;
        payloadChecksum = false;
    }
}

/*!
 * \brief Message::checkPayloadChecksum
 */
void Message::checkPayloadChecksum() {
    if ( payloadHasChecksum() ) {
        payloadValid = CRC_16::checkCRC( messagePacket.payloadData.constData() );
    }
}

/*!
 * \brief Message::payloadHasChecksum
 * \return
 */
bool Message::payloadHasChecksum() const {
    return payloadChecksum;
}

/*!
 * \brief Message::addHeaderChecksum
 */
void Message::addHeaderChecksum() {
    if ( headerHasChecksum() ) {
        return;
    }

    DataArray headerWithCrc( CRC_HEC::addCRC( messagePacket.headerData.constData() ) );

    messagePacket.headerData = headerWithCrc;
    headerChecksum = true;
}

/*!
 * \brief Message::removeHeaderChecksum
 */
void Message::removeHeaderChecksum() {
    if ( !headerHasChecksum() ) {
        return;
    }

    DataArray headerWithoutCRC( CRC_HEC::removeCRC( messagePacket.headerData.constData() ) );

    messagePacket.headerData = headerWithoutCRC;
    headerChecksum = false;
}

/*!
 * \brief Message::packHeader
 */
void Message::packHeader() {
    addHeaderChecksum();
    addHeaderFEC();
}

/*!
 * \brief Message::unpackHeader
 */
void Message::unpackHeader() {
    removeHeaderFEC();
    checkHeaderChecksum();
    removeHeaderChecksum();
}

/*!
 * \brief Message::isHeaderValid
 * \return
 */
bool Message::isHeaderValid() const {
    return headerValid;
}

/*!
 * \brief Message::addHeaderFEC
 */
void Message::addHeaderFEC() {

    if ( headerHasFEC() ) {
        return;
    }

    DataArray hecHeader( messagePacket.headerData.size() * 3 );
    for ( quint32 index = 0, indexForHec = 0; index < messagePacket.headerData.size(); ++ index, indexForHec += 3 ) {
        hecHeader.set( indexForHec,       messagePacket.headerData.at( index ) );
        hecHeader.set( indexForHec + 1,   messagePacket.headerData.at( index ) );
        hecHeader.set( indexForHec + 2,   messagePacket.headerData.at( index ) );
    }

    messagePacket.headerData = hecHeader;
    headerFEC = true;
}

/*!
 * \brief Message::removeHeaderFEC
 */
void Message::removeHeaderFEC() {
    if ( ! headerHasFEC() ) {
        return;
    }

    DataArray unHecHeader( messagePacket.headerData.size() / 3 );
    for ( quint32 index = 0, indexForUnHec = 0; index < messagePacket.headerData.size(); index += 3, ++ indexForUnHec ) {
        // 0 ----
        // 000
        // 001
        // 010
        // 100

        // 1 ----
        // 101
        // 110
        // 111

        auto oneCounter = 0;
        if ( messagePacket.headerData.at( index ) ) {
            oneCounter ++;
        }
        if ( messagePacket.headerData.at( index + 1 ) ) {
            oneCounter ++;
        }
        if ( messagePacket.headerData.at( index + 2 ) ) {
            oneCounter ++;
        }

        if ( oneCounter >= 2 ) {
            unHecHeader.set( indexForUnHec, true );
        }
        else {
            unHecHeader.set( indexForUnHec, false );
        }
    }

    messagePacket.headerData = unHecHeader;
    headerFEC = false;
}

/*!
 * \brief Message::packPayload
 */
void Message::packPayload() {
    addPayloadChecksum();
}

/*!
 * \brief Message::unpackPayload
 */
void Message::unpackPayload() {
    checkPayloadChecksum();
    removePayloadChecksum();
}

/*!
 * \brief Message::checkHeaderChecksum
 */
void Message::checkHeaderChecksum() {
    if ( headerHasChecksum() ) {
        headerValid = CRC_HEC::checkCRC( messagePacket.headerData.constData() );
    }
}

/*!
 * \brief Message::headerIsPacked
 * \return
 */
bool Message::headerIsPacked() const {
    return ( headerHasFEC() && headerHasChecksum() );
}

/*!
 * \brief Message::isPayloadValid
 * \return
 */
bool Message::isPayloadValid() const {
    return payloadValid;
}

/*!
 * \brief Message::addInformationToPayload
 * \param information
 */
void Message::addInformationToPayload( const QByteArray &information ) {
    removePayloadChecksum();

    if( quint32( information.size()*8 ) <= PAYLOAD_MAX_INFORMATION_LENGHT ) {

        setPlayloadInformationSize( information.size() );

        // QByteArray to QBitArray
        QBitArray bitInformation( information.size() * 8 );
        for( auto i = 0; i < information.size(); ++i ) {
            for( auto b = 0; b < 8; ++b ){
                bitInformation.setBit( i * 8 + b, information.at(i) & ( 1 << b ) );
            }
        }

        QBitArray & bitArrayRef( messagePacket.payloadData.data() );

        for ( qint32 payloadIndex = bitArrayRef.size() - 1 - PAYLOAD_HEADER_LENGHT,
              informationIndex = bitInformation.size() - 1;
              informationIndex != -1; -- payloadIndex, -- informationIndex ) {
            bitArrayRef.setBit( payloadIndex, bitInformation.at( informationIndex ) );
        }

    }
    else {
        qWarning() << "Information packet is too big : " << information.size() << " bytes";
    }

    addPayloadChecksum();
}

/*!
 * \brief Message::getInformationFromPayload
 * \return
 */
QByteArray Message::getInformationFromPayload() {
    removePayloadChecksum();
    auto informationSize = getPlayloadInformationSize();

    QByteArray resultArray( informationSize, '\0' );

    QBitArray & bitArrayRef( messagePacket.payloadData.data() );

    // Get bitData

    QBitArray bitInformation( informationSize * 8 );
    for ( qint32 payloadIndex = bitArrayRef.size() - 1 - PAYLOAD_HEADER_LENGHT,
          informationIndex = bitInformation.size() - 1;
          informationIndex != -1; -- payloadIndex, -- informationIndex ) {
        bitInformation.setBit( informationIndex, bitArrayRef.at( payloadIndex ) );
    }

    for( quint32 b = 0; b < quint32( bitInformation.size() ); ++b ) {
        resultArray[ b / 8 ] = ( resultArray.at( b / 8 ) | ( bitInformation[ b ] ? 1 : 0 ) << ( b % 8 ) );
    }

    addPayloadChecksum();
    return resultArray;
}

/*!
 * \brief Message::setPlayloadInformationSize
 * \param informationSize
 */
void Message::setPlayloadInformationSize( quint32 informationSize ) {
    removePayloadChecksum();

    QBitArray & payloadRef( messagePacket.payloadData.data() );

    auto bitSize = QBitArrayHelper::uintToQBitArray< PAYLOAD_DATA_LENGHT > ( informationSize );
    for ( qint32 sizeIndex = bitSize.size() - 1, payloadIndex = payloadRef.size() - 1 - PAYLOAD_L_CH_LENGHT - PAYLOAD_FLOW_LENGHT; sizeIndex != -1; -- sizeIndex, -- payloadIndex  ) {
        payloadRef.setBit( payloadIndex, bitSize.at( sizeIndex ) );
    }

    addPayloadChecksum();
}

/*!
 * \brief Message::getPlayloadInformationSize
 * \return
 */
quint32 Message::getPlayloadInformationSize() {
    removePayloadChecksum();

    QBitArray & payloadRef( messagePacket.payloadData.data() );

    QBitArray bitInformationSize( PAYLOAD_DATA_LENGHT );
    for ( qint32 sizeIndex = bitInformationSize.size() - 1, payloadIndex = payloadRef.size() - 1 - PAYLOAD_L_CH_LENGHT - PAYLOAD_FLOW_LENGHT; sizeIndex != -1; -- sizeIndex, -- payloadIndex  ) {
        bitInformationSize.setBit( sizeIndex, payloadRef.at( payloadIndex ) );
    }
    auto informationSize = QBitArrayHelper::qBitArrayToUint( bitInformationSize );

    addPayloadChecksum();
    return informationSize;
}



#ifdef MESSAGE_TEST
#include <QtTest/QtTest>

class MessageTest : public QObject {
    Q_OBJECT

private slots:
    void initTestCase() { /*Empty*/ }

    // Here you can implement your own test slots
    void setAM_ADD_RTest () {
        Message message;
        auto addr = 0b011;
        message.setAM_ADDR( addr );

        auto gettedAddr = message.getAM_ADDR();
        QVERIFY( addr == gettedAddr );
    }

    void setTYPE_Test () {
        Message message;
        auto type = 0b1101;
        message.setTYPE( type );

        auto gettedType = message.getTYPE();
        QVERIFY( type == gettedType );
    }

    void setFLOW_Test () {
        Message message;
        auto flow = true;
        message.setFLOW( flow );

        auto gettedFlow = message.getFLOW();
        QVERIFY( flow == gettedFlow );
    }

    void payload_Test () {
        Message message;

        QByteArray information( "This is sparta!" );
        message.addInformationToPayload( information );

        auto receivedInformation = message.getInformationFromPayload();

        QVERIFY( information == receivedInformation );
    }

    void cleanupTestCase() { /*Empty*/ }
};

QTEST_MAIN( MessageTest )
#include "Message.moc"

#endif

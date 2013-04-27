#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <QBitArray>

#include "Defines.hpp"

#include "RawDataWrapper.hpp"

/*!
 * \brief DataArray
 */
typedef RawDataWrapper<QBitArray> DataArray;

#include "CRC.hpp"

constexpr quint32 ACCESS_CODE_LENGHT = 72;

constexpr quint32 AM_ADDR_LENGHT = 3;
constexpr quint32 TYPE_LENGHT = 4;
constexpr quint32 FLOW_LENGHT = 1;
constexpr quint32 ARQN_LENGTH = 1;
constexpr quint32 SEQN_LENGTH = 1;
constexpr quint32 HEADER_LENGHT = AM_ADDR_LENGHT + TYPE_LENGHT + FLOW_LENGHT + ARQN_LENGTH + SEQN_LENGTH; // In common 54


constexpr quint32 PAYLOAD_L_CH_LENGHT = 2;
constexpr quint32 PAYLOAD_FLOW_LENGHT = 1;
constexpr quint32 PAYLOAD_DATA_LENGHT = 5;
constexpr quint32 PAYLOAD_HEADER_LENGHT = PAYLOAD_L_CH_LENGHT + PAYLOAD_FLOW_LENGHT + PAYLOAD_DATA_LENGHT; // 8

constexpr quint32 PAYLOAD_MAX_INFORMATION_LENGHT = 17*8; // 17 bytes max playload for DM1
constexpr quint32 CRC_16_LENGHT = 16; // CRC - 16
constexpr quint32 PAYLOAD_LENGHT = PAYLOAD_HEADER_LENGHT + PAYLOAD_MAX_INFORMATION_LENGHT + CRC_16_LENGHT;

typedef CRC< HEADER_LENGHT, 8, 0x7 > CRC_HEC; // CRC Header Error Check
typedef CRC< PAYLOAD_LENGHT - CRC_16_LENGHT, CRC_16_LENGHT, 0x8005 > CRC_16; // CRC - 16 for playload checksum
typedef CRC< 0, 0, 0 > QBitArrayHelper;

#define ALL_OK 0
#define HEADER_ERROR 1
#define PAYLOAD_ERROR 2
#define HEADER_AND_PAYLOAD_ERROR 3


/*!
 * \brief The Message class
 */
class Message : public QObject {
    Q_OBJECT
private:
    /*!
     * \brief The Packet class
     */
    class Packet {
    public:
        /*!
         * \brief Packet
         * \param accessCodeSize
         * \param headerSize
         * \param payloadSize
         */
        Packet( const quint32 & accessCodeSize, const quint32 & headerSize, const quint32 & payloadSize ) :
            accessCodeData( accessCodeSize ),
            headerData( headerSize ),
            payloadData( payloadSize )
        { /*Empty*/ }


        /*!
         * \brief Packet
         * \param access
         * \param header
         * \param payload
         */
        Packet( const DataArray & access, const DataArray & header, const DataArray & payload ) :
            accessCodeData( access ),
            headerData( header ),
            payloadData( payload )
        { /*Empty*/ }

    public:
        DataArray accessCodeData;
        DataArray headerData;
        DataArray payloadData;
    };

public:
    Message();
    Message( const Message & message );

    Message & operator = ( const Message & r_message );

    const Packet & constPacket() const;
    Packet & packet();
    void clear();

    // Message methods
    void packMessage();
    void unpackMessage();

    int isValid() const;

    // Header methods
    bool isHeaderValid() const;

    // Header state methods
    bool headerIsPacked() const;
    bool headerHasFEC() const;
    bool headerHasChecksum() const;

    void setAM_ADDR( quint8 am_addr );
    void setTYPE( quint8 type );
    void setFLOW( bool flow );
    void setARQN( bool arqn );
    void setSEQN( bool seqn );

    quint8 getAM_ADDR();
    quint8 getTYPE();
    bool getFLOW();
    bool getARQN();
    bool getSEQN();


    // Payload methods
    bool payloadHasChecksum() const;
    bool isPayloadValid() const;

    void addInformationToPayload(const QByteArray &information );
    QByteArray getInformationFromPayload();

    // Forbidden operators
    bool operator == ( const Message &  ) = delete;
    bool operator != ( const Message &  ) = delete;

private:

    // Header
    void packHeader();
    void unpackHeader();


    // Header methods

    // CRC_HEC checksum
    void addHeaderChecksum();
    void removeHeaderChecksum();
    void checkHeaderChecksum();

    // FEC 1/3
    void addHeaderFEC();
    void removeHeaderFEC();


    // Payload
    void packPayload();
    void unpackPayload();

    void addPayloadChecksum();
    void removePayloadChecksum();
    void checkPayloadChecksum();

    void setPlayloadInformationSize(quint32 informationSize );
    quint32 getPlayloadInformationSize();

private:
    // Header state flags
    bool headerFEC;
    bool headerChecksum;

    bool headerValid;

    // Playload state flags

    bool payloadChecksum;
    bool payloadValid;
    Packet messagePacket;
};

#endif // MESSAGE_HPP

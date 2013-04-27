#include "CRC.hpp"
#include "Defines.hpp"

#ifdef CRC_TEST

#include <QDebug>
#include "Message.hpp"
#include "Rand.hpp"
#include "Environment.hpp"

constexpr quint32 MAX_NUMBER_OF_ERRORS = 40;

void CRC_HEC_Cottupt_Test(const quint16 testData) {

    auto crc_hec = CRC_HEC::addCRC( CRC_HEC::informatonToQBitArray( testData ) );
    qDebug() << CRC_HEC::QBitArrayToQString( crc_hec );

    // from 0 to MAX_NUMBER_OF_ERRORS error
    auto corruptCounter = Rand::randInRange( MAX_NUMBER_OF_ERRORS );
    qDebug() << "Make " << corruptCounter << " errors";
    for ( ; corruptCounter != 0; -- corruptCounter ) {
//        qDebug() << "Corrupt bit #" << Environment::corrupt( crc_hec );;
        auto checkResult = CRC_HEC::checkCRC( crc_hec );
        if ( checkResult ) {
            qDebug() << "Check crc = true" <<  checkResult;
        }
    }
}


void CRC_16_Corrupt_Test(const quint32 testData)
{
    auto crc_hec = CRC_16::addCRC( CRC_16::informatonToQBitArray( testData ) );
    qDebug() << CRC_HEC::QBitArrayToQString( crc_hec );

    // from 0 to MAX_NUMBER_OF_ERRORS error
    auto corruptCounter = Rand::randInRange( MAX_NUMBER_OF_ERRORS );
    qDebug() << "Make " << corruptCounter << " errors";
    for ( ; corruptCounter != 0; -- corruptCounter ) {
//        qDebug() << "Corrupt bit #" << CRC_16::corrupt( crc_hec );
        auto checkResult = CRC_16::checkCRC( crc_hec );
        if ( checkResult ) {
            qDebug() << "Check crc = true" <<  checkResult;
        }
    }
}


void CRC_HEC_Test() {
    qDebug() << "\nCRC_HEC_Test();";
    const quint16 testData = 0x57;

    auto crc_hec = CRC_HEC::addCRC( CRC_HEC::informatonToQBitArray( testData ) );
    qDebug() << CRC_HEC::QBitArrayToQString( crc_hec );

    qDebug() << "Check removeCRC";
    auto qBitTestData = CRC_HEC::informatonToQBitArray( testData );
    auto crc_Remove_crc = CRC_HEC::removeCRC( crc_hec );

    if (  qBitTestData !=  crc_Remove_crc ) {
        qDebug() << "Fail!";
        qDebug() << "qBitTestData = " << CRC_HEC::QBitArrayToQString( qBitTestData );
        qDebug() << "crc_Remove_crc = " << CRC_HEC::QBitArrayToQString( crc_Remove_crc );
    }


//    CRC_HEC_Cottupt_Test(testData);

    qDebug() << endl;

}

void CRC_16_Test() {
    qDebug() << "\nCRC_HEC_Test();";
    const quint32 testData = 0xDEADBEEF;

    auto crc_hec = CRC_16::addCRC( CRC_16::informatonToQBitArray( testData ) );
    qDebug() << CRC_16::QBitArrayToQString( crc_hec );

    qDebug() << "Check removeCRC";
    if (  CRC_16::informatonToQBitArray( testData ) != CRC_16::removeCRC( crc_hec ) ) {
        qDebug() << "Fail!";
    }

//    CRC_16_Corrupt_Test(testData);

    qDebug() << endl;

}


int main() {
    qDebug() << "CRC test";
    CRC_HEC_Test();
    CRC_16_Test();
}

#endif

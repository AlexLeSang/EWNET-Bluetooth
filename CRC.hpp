#ifndef CRC_HPP
#define CRC_HPP

#include <QBitArray>
#include <QString>
#include <QStringBuilder>
#include <QTime>

#include <QDebug>

#include "Defines.hpp"

template < quint32 informationLength, quint32 crcLenght, quint32 polynomValue >
/*!
 * \brief The CRC class
 */
class CRC {
public:

    /*!
     * \brief checkCRC
     * \param crcArray
     * \return
     */
    static bool checkCRC( const QBitArray & crcArray ) {
        QBitArray arrayForRecalculate( informationLength );

        //        qDebug() << "crcArray            = " << qBitArrayToQString( crcArray );

        for ( quint32 crcIndex = crcArray.size() - 1, recalculateIndex = informationLength - 1; crcIndex != crcLenght - 1; --crcIndex, --recalculateIndex ) {
            arrayForRecalculate.setBit( recalculateIndex, crcArray.at( crcIndex ) );
        }

        //        qDebug() << "arrayForRecalculate = " << qBitArrayToQString( arrayForRecalculate );

        return crcArray == addCRC( arrayForRecalculate );

    }

    /*!
     * \brief addCRC
     * \param array
     * \return
     */
    static QBitArray addCRC( const QBitArray & array ) {

        Q_ASSERT( array.size() == informationLength );

        QBitArray result( informationLength + crcLenght ); // array + crc
        // put array and zero crc
        for ( auto resultIndex = result.size() - 1, arrayIndex = array.size() - 1; arrayIndex != -1; -- arrayIndex, -- resultIndex ) {
            result.setBit( resultIndex, array.at( arrayIndex ) );
        }

        QBitArray polynom = uintToQBitArray< crcLenght + 1 > ( polynomValue );
        polynom.setBit( polynom.size() - 1, true ); // set MSB bit

        while ( result.size() != crcLenght ) {
            // MSB == 1
            if ( result.at( result.size() - 1 ) ) {
                for ( auto resultIndex = result.size() - 1, polynomIndex = polynom.size() - 1; polynomIndex != -1; --resultIndex, --polynomIndex ) {
                    result[ resultIndex ] = result.at( resultIndex ) ^ polynom.at( polynomIndex );
                }
            }
            // MSB == 0
            else { /* Empty */ }

            // Use this like <<
            result.truncate( result.size() - 1 );
        }

        // Resize for result
        result.resize( result.size() + array.size() );
        for ( auto resultIndex = result.size() - 1, arrayIndex = array.size() - 1; arrayIndex != -1; -- arrayIndex, -- resultIndex ) {
            result.setBit( resultIndex, array.at( arrayIndex ) );
        }

        return result;
    }

    /*!
     * \brief removeCRC
     * \param array
     * \return
     */
    static QBitArray removeCRC( const QBitArray & array ) {
        QBitArray result( informationLength );
        for ( auto indexInput = array.size() - 1, indexResult = result.size() - 1; indexResult != -1; --indexInput, -- indexResult ) {
            result.setBit( indexResult, array.at( indexInput ) );
        }
        return result;
    }

    /*!
     * \brief uintToQBitArray
     * \param number
     * \return
     */
    template < quint32 resultSize >
    static QBitArray uintToQBitArray( quint32 number ) {
        QBitArray array( resultSize, false );
        for ( int i = 0; i < array.size(); ++i ) {
            // Check with bit mask
            if ( number & ( 1 << i ) ) {
                array.setBit( i, true );
            }
            else {
                array.setBit( i, false );
            }
        }
        return array;
    }

    /*!
     * \brief informatonToBitArray
     * \param number
     * \return
     */
    static QBitArray informatonToBitArray( quint32 number ) {
        return uintToQBitArray< informationLength >( number );
    }

    /*!
     * \brief qBitArrayToUint16
     * \param array
     * \return
     */
    static quint32 qBitArrayToUint( const QBitArray & array ) {
        quint32 result = 0;
        for ( auto i = 0; i < array.size(); ++i ) {
            result |= array.at( i ) << i;
        }
        return result;
    }

    /*!
     * \brief qBitArrayToQString
     * \param array
     * \return
     */
    static QString qBitArrayToQString( const QBitArray & array ) {
        QString string;
        for ( auto i = 0; i < array.size(); ++i ) {
            if ( false == array.at( i ) ) {
                string = QChar('0') % string;
            }
            else {
                string = QChar('1') % string;
            }
        }
        return string;
    }

};

#endif // CRC_HPP

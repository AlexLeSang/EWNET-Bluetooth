#ifndef RAWDATAWRAPPER_HPP
#define RAWDATAWRAPPER_HPP

#include <QString>

template <typename T>
/*!
 * \brief The RawDataWrapper class wrapper for raw pointer on data structure
 */
class RawDataWrapper {
public:
    /*!
     * \brief RawDataWrapper
     * \param dataSize
     */
    RawDataWrapper( quint32 dataSize = 0 ) : rawDataPtr( new T( dataSize, 0 ) ) { /*Empty*/ }

    /*!
     * \brief RawDataWrapper
     * \param rawArray
     */
    RawDataWrapper( const T & rawArray ) : rawDataPtr( new T( rawArray ) ) { /*Empty*/ }

    /*!
     * \brief RawDataWrapper
     * \param dataArray
     */
    RawDataWrapper( const RawDataWrapper & dataArray ) : rawDataPtr ( new T( dataArray.constData() ) ) { /*Empty*/ }

    /*!
     * \brief ~RawDataWrapper
     */
    ~RawDataWrapper() {
        if ( nullptr != rawDataPtr ) {
            delete rawDataPtr;
        }
    }

    /*!
     * \brief data
     * \return
     */
    T & data() {
        return *rawDataPtr;
    }

    /*!
     * \brief constData
     * \return
     */
    const T & constData() const {
        return *rawDataPtr;
    }

    /*!
     * \brief clear
     */
    void clear() {
        rawDataPtr->fill('0');
    }

    /*!
     * \brief size
     * \return
     */
    quint32 size() const {
        return rawDataPtr->size();
    }

    /*!
     * \brief set
     * \param i
     * \param val
     */
    void set ( quint32 i, uchar val ) {
         rawDataPtr->operator[] ( i ) = val;
    }

    /*!
     * \brief at
     * \param i
     */
    decltype( T.at( 0 ) ) at( quint32 i ) const {
        return rawDataPtr->at( i );
    }

    /*!
     * \brief operator =
     * \param dataArray
     * \return
     */
    RawDataWrapper & operator = ( const RawDataWrapper & dataArray ) {
        if ( nullptr != rawDataPtr ) {
            delete rawDataPtr;
        }
        rawDataPtr = new T( dataArray.constData() );
        return *this;
    }

    /*!
     * \brief operator ==
     * \param r_dataArray
     * \return
     */
    bool operator == ( const RawDataWrapper &  r_dataArray ) const {
        return ( *rawDataPtr == r_dataArray.constData() );
    }

    /*!
     * \brief operator !=
     * \param r_dataArray
     * \return
     */
    bool operator != ( const RawDataWrapper & r_dataArray ) const {
        return ! ( *this == r_dataArray );
    }

private:
    T * rawDataPtr;
};

#include <QDebug>
template < typename T>
/*!
 * \brief operator <<
 * \param qdebug
 * \param wrapper
 * \return
 */
QDebug operator << ( QDebug qdebug, const RawDataWrapper<T> & wrapper ) {
    for ( quint32 index = 0; index < wrapper.size(); ++ index ) {
        qdebug.nospace() << wrapper.at( index );
    }
    return qdebug;
}

#endif // RAWDATAWRAPPER_HPP

#ifndef RAND_HPP
#define RAND_HPP

#include <QTime>

#include "Defines.hpp"

/*!
 * \brief The Rand class
 */
class Rand {
public:
    /*!
     * \brief randInRange
     * \param max
     * \return
     */
    static qint32 randInRange( qint32 max ) {
        static bool seeded = false;
        if ( !seeded ) {
            qsrand( QTime::currentTime().msec() );
            seeded = true;
        }
        return ( qrand() + 1 ) % max;
    }


    /*!
     * \brief randQReal
     * \return
     */
    static qreal randQReal() {
        static bool seeded = false;
        if ( !seeded ) {
            qsrand( QTime::currentTime().msec() );
            seeded = true;
        }
        return qreal( qreal( qrand() ) / RAND_MAX );
    }
};

#ifdef RAND_TEST

#include <QDebug>
int main() {
    quint32 repeatCounter = 100;
    while( repeatCounter-- ) {
        qDebug() << Rand::randQReal();
    }
}

#endif

#endif // RAND_HPP



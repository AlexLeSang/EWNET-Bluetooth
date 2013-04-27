#ifndef SLEEP_HPP
#define SLEEP_HPP

#include <QThread>

/*!
 * \brief The Sleep class
 */
class Sleep : QThread {
public:
    /*!
     * \brief usleep
     * \param usecs
     */
    static void usleep( unsigned long usecs ) {
        QThread::usleep( usecs );
    }

    /*!
     * \brief msleep
     * \param msecs
     */
    static void msleep( unsigned long msecs ) {
        QThread::msleep( msecs );
    }

    /*!
     * \brief sleep
     * \param secs
     */
    static void sleep( unsigned long secs ) {
        QThread::sleep( secs );
    }

private:
    Sleep() = delete;
};

#endif // SLEEP_HPP

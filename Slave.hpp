#ifndef SLAVE_HPP
#define SLAVE_HPP

#include "BluetoothNode.hpp"

/*!
 * \brief The Slave class
 */
class Slave : public BluetoothNode {
public:
    static Slave & getInstance();
    ~Slave();

    Slave( const Slave &  ) = delete;
    Slave & operator = ( const Slave & ) = delete;
    bool operator == ( const Slave &  ) = delete;
    bool operator != ( const Slave &  ) = delete;

    void reinit();
private:
    explicit Slave();

private:
    const quint32 SLAVE_AM_ADDR = 0x001;
};
#endif // SLAVE_HPP

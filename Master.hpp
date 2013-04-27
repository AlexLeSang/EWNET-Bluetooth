#ifndef MASTER_HPP
#define MASTER_HPP

#include "BluetoothNode.hpp"

/*!
 * \brief The Master class
 */
class Master : public BluetoothNode {
public:
    static Master & getInstance();
    ~Master();

    Master( const Master &  ) = delete;
    Master & operator = ( const Master & ) = delete;
    bool operator == ( const Master &  ) = delete;
    bool operator != ( const Master &  ) = delete;

    void reinit();
private:
    explicit Master();
private:
    const quint32 MASTER_AM_ADDR = 0x000;

};

#endif // MASTER_HPP

#include "Slave.hpp"

#include "SimulationManager.hpp"

/*!
 * \brief Slave::getInstance
 * \return
 */
Slave &Slave::getInstance() {
    static Slave instance;
    return instance;
}

/*!
 * \brief Slave::~Slave
 */
Slave::~Slave() { /*Empty*/ }

/*!
 * \brief Slave::reinit
 */
void Slave::reinit() {
    BluetoothNode::reinit();

    Message constantSlaveMessage;
    constantSlaveMessage.setAM_ADDR( SLAVE_AM_ADDR );
    for ( qint32 i = SimulationManager::getMAX_CYCLES(); i != 0; --i ) {
        auto information = QByteArray( "Slave #" + QString::number(i).toLocal8Bit() );
        information.reserve( PAYLOAD_MAX_INFORMATION_LENGHT / 8 );
        constantSlaveMessage.addInformationToPayload( information );
        addMessageToSend( constantSlaveMessage );
    }
}

/*!
 * \brief Slave::Slave
 */
Slave::Slave() {
    nameString = "Slave";
    addToLog = &CoreFacade::sendToSlaveLog;
    reinit();
}
